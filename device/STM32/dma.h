#pragma once
#include <lmcu/device>

namespace lmcu {
namespace dma {

enum class module_id
{
#if defined(DMA1)
  dma1
#endif
};

enum class mode { mem_mem, mem_periph };

enum class prio { low, medium, high, very_high };

enum class mem_word { _8bit, _16bit, _32bit };

enum class periph_word { _8bit, _16bit, _32bit };

enum class mem_inc { enable, disable };

enum class periph_inc { enable, disable };

enum class circular { enable, disable };

enum class dir { read_periph, read_mem };

enum class channel { ch1, ch2, ch3, ch4, ch5, ch6, ch7 };

template<
  module_id _module_id,
  channel _channel,
  mode _mode = mode::mem_periph,
  mem_word _mem_word = mem_word::_8bit,
  periph_word _periph_word = periph_word::_8bit,
  mem_inc _mem_inc = mem_inc::enable,
  periph_inc _periph_inc = periph_inc::disable,
  dir _dir = dir::read_periph,
  circular _circular = circular::enable,
  prio _prio = prio::low
>
struct module
{
  static constexpr auto module_id   = _module_id;
  static constexpr auto channel     = _channel;
  static constexpr auto mode        = _mode;
  static constexpr auto mem_word    = _mem_word;
  static constexpr auto periph_word = _periph_word;
  static constexpr auto mem_inc     = _mem_inc;
  static constexpr auto periph_inc  = _periph_inc;
  static constexpr auto dir         = _dir;
  static constexpr auto circular    = _circular;
  static constexpr auto prio        = _prio;
};

#include "detail/dma.h"

template<typename ...args>
void configure() { detail::configure<args...>(); }

template<typename _conf>
void stop() { detail::stop<_conf>(); }

template<typename _conf, typename _src_t, typename _dst_t>
void start(_src_t&& src, _dst_t&& dst, uint16_t size) { detail::start<_conf>(src, dst, size); }

} // namespace dma
} // namespace lmcu