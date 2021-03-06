#pragma once
#include <lmcu/dev/mcu>
#include <lmcu/dev/cortex/nvic>
#include <lmcu/common>

// ------------------------------------------------------------------------------------------------
namespace lmcu::irq {
// ------------------------------------------------------------------------------------------------

/**
 * @brief Enable global interrupts.
*/
lmcu_static_inline void enable() noexcept { asm volatile("cpsie i" ::: "memory"); }

/**
 * @brief Disable global interrupts.
*/
lmcu_static_inline void disable() noexcept { asm volatile("cpsid i" ::: "memory"); }

/**
 * @brief Return interrupt priority mask.
*/
lmcu_static_inline uint32_t primask() noexcept
{
  uint32_t r;
  asm volatile("MRS %0, primask" : "=r" (r));
  return r;
}

class ctl
{
  ctl(const ctl&) = delete;
  ctl &operator =(const ctl&) = delete;
public:
  lmcu_inline ctl() noexcept { }

  /**
   * @brief Force enable global interrupts.
  */
  lmcu_inline void enable() noexcept
  {
    irqen_ = false; irq::enable();
  }

  /**
   * @brief Enables global interrupts if them been enabled, before call 'ctl::disable'.
  */
  lmcu_inline void enable_if_needed() noexcept
  {
    if(irqen_) { irqen_ = false; irq::enable(); }
  }

  /**
   * @brief Disable global interrupts.
  */
  lmcu_inline void disable() noexcept
  {
    irqen_ = irq::primask() == 0;
    irq::disable();
  }

  lmcu_inline ~ctl() noexcept
  {
    enable_if_needed();
  }
private:
  bool irqen_ = false;
};

// ------------------------------------------------------------------------------------------------
} // namespace lmcu::irq
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
namespace lmcu::nvic {
// ------------------------------------------------------------------------------------------------

enum class preempt_prio : uint8_t { defval = 14 };
enum class sub_prio : uint8_t { defval = 0 };
enum class group_prio : uint8_t { defval = 0 };

struct irq_conf
{
  preempt_prio preempt;
  sub_prio sub;
  group_prio group;
};

template<typename _irq_cfg, auto ..._args>
constexpr inline _irq_cfg encode_irq_conf() noexcept
{
  constexpr auto pp = option::get_u<preempt_prio, _args...>(preempt_prio::defval);
  constexpr auto sp = option::get_u<sub_prio, _args...>(sub_prio::defval);
  constexpr auto gp = option::get_u<group_prio, _args...>(group_prio::defval);

  static_assert(option::check<
    std::tuple<preempt_prio, sub_prio, group_prio>,
    _args...
  >());

  return _irq_cfg((uint32_t(pp) << 16) | (uint32_t(sp) << 8) | uint32_t(gp));
}

template<typename _irq_cfg>
constexpr inline auto decode_irq_conf(_irq_cfg irq_id) noexcept
{
  auto r = uint32_t(irq_id);

  return irq_conf {
    preempt_prio((r >> 16) & 0xff), sub_prio((r >> 8) & 0xff), group_prio(r & 0xff)
  };
}

/**
 * @brief Enable IRQ vectors.
 *
 * @tparam _irq_n - IRQ numbers.
*/
template<device::irqn ..._irq_n>
void enable_irq()
{
  using namespace device;

  auto irq_e = [](auto irq)
  {
    if(irq >= 0) { NVIC::ISER::set(irq >> 5UL, 1UL << (irq & 0x1fUL)); }
  };

  (irq_e(uint32_t(_irq_n)), ...);
}

/**
 * @brief Disable IRQ vectors.
 *
 * @tparam _irq_n - IRQ numbers.
*/
template<device::irqn ..._irq_n>
void disable_irq()
{
  using namespace device;

  auto irq_d = [](auto irq)
  {
    if(irq >= 0) { NVIC::ICER::set(irq >> 5UL, 1UL << (irq & 0x1fUL)); }
  };

  (irq_d(uint32_t(_irq_n)), ...);
}

/**
 * @brief Set IRQ priority.
 *
 * @tparam _irq_n   - IRQ number.
 * @tparam _irq_cfg - encoded IRQ priority config.
*/
template<device::irqn _irq_n, auto _irq_cfg>
void set_encoded_priority()
{
  using namespace device;

  if constexpr(_irq_n != irqn::invalid_irqn) {
    constexpr auto cfg = decode_irq_conf(_irq_cfg);

    constexpr uint32_t priority_group = uint32_t(cfg.group) & 0x7UL;

    constexpr uint32_t preempt_prio_bits =
      ((7 - priority_group) > nvic_prio_bits)? nvic_prio_bits : 7 - priority_group
    ;

    constexpr uint32_t sub_prio_bits =
      ((priority_group + nvic_prio_bits) < 7)? 0 : (priority_group - 7) + nvic_prio_bits
    ;

    constexpr auto priority =
      ((uint32_t(cfg.preempt) & ((1UL << preempt_prio_bits) - 1UL)) <<  sub_prio_bits) |
      (uint32_t(cfg.sub) & ((1UL << sub_prio_bits) - 1UL))
    ;

    if constexpr(int32_t(_irq_n) < 0) {
      NVIC::SHPR::set(
        (uint32_t(_irq_n) & 0xfUL) - 4UL, (priority << (8UL - nvic_prio_bits)) & 0xffUL
      );
    }
    else {
      NVIC::IPR::set(uint32_t(_irq_n), (priority << (8UL - nvic_prio_bits)) & 0xffUL);
    }
  }
}

/**
 * @brief Set IRQ priority.
 *
 * @tparam _irq_n - IRQ number.
 * @tparam _args  - Any combination of 'preempt_prio', 'sub_prio', 'group_prio' values.
*/
template<device::irqn _irq_n, auto ..._args>
lmcu_inline void set_priority()
{ set_encoded_priority<_irq_n, encode_irq_conf<uint32_t, _args...>()>(); }

/**
 * @brief Set library default IRQ priority.
*/
template<device::irqn _irq_n>
lmcu_inline void set_default_priority()
{ set_priority<_irq_n, preempt_prio::defval, sub_prio::defval, group_prio::defval>(); }

// ------------------------------------------------------------------------------------------------
} // namespace lmcu::nvic
// ------------------------------------------------------------------------------------------------
