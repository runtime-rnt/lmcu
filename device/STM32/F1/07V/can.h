#pragma once

namespace lmcu {
namespace can {

enum class remap
{
  none = -1,
  CAN1__rx_PB8_tx_PB9 = AFIO_MAPR_CAN_REMAP_REMAP2,
  CAN1__rx_PD0_tx_PD1 = AFIO_MAPR_CAN_REMAP_REMAP3,
  CAN2__rx_PB5_tx_PB6 = AFIO_MAPR_CAN2_REMAP
};

} // namespace can
} // namespace lmcu


#include "../can.h"

namespace lmcu {
namespace can {

namespace detail {

template<uint32_t r, typename arg1, typename ...args>
uint32_t remap_bits()
{
  if constexpr(sizeof...(args) > 0) { return remap_bits<r, args...>(); }
  constexpr auto m = arg1();

  if constexpr(m.module_id == module_id::can1) {
    static_assert(
      m.remap == remap::none ||
      m.remap == remap::CAN1__rx_PB8_tx_PB9 ||
      m.remap == remap::CAN1__rx_PD0_tx_PD1,
      "invalid value of remap argument"
    );
  }

  if constexpr(m.module_id == module_id::can2) {
    static_assert(
      m.remap == remap::none ||
      m.remap == remap::CAN2__rx_PB5_tx_PB6,
      "invalid value of remap argument"
    );
  }

  if constexpr(m.remap != remap::none) { return r | static_cast<uint32_t>(m.remap); }
  return r;
}

} // namespace detail

} // namespace can
} // namespace lmcu
