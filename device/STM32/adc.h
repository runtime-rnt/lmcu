#pragma once
#include <lmcu/rcc>
#include <lmcu/delay>

namespace lmcu {
namespace adc {

enum class module_id
{
#if defined(ADC1)
  adc1,
#endif

#if defined(ADC2)
  adc2,
#endif

#if defined(ADC3)
  adc3,
#endif

#if defined(ADC4)
  adc4
#endif
};

enum class dual_mode
{
  independent,
  regular_and_injected,
  regular_and_trigger,
  injected_and_fast_inter,
  injected_and_slow_inter,
  injected_only,
  regular_only,
  fast_inter_only,
  slow_inter_only,
  trigger_only
};

enum class inj_discont_mode { enable, disable };

enum class reg_discont_mode { enable, disable };

enum class inj_auto { enable, disable };

enum class scan { enable, disable };

enum class temp_sensor { enable, disable };

enum class reg_trig
{
  disable,

#if defined(ADC1)
  tim1_cc1,
  tim1_cc2,
  tim1_cc3,
  tim2_cc2,
  tim3_trgo,
  tim4_cc4,
  exti11__tim8_trgo,
#endif

  sw_start,

#if defined(ADC3)
  tim3_cc1,
  tim2_cc3,
  tim1_cc3,
  tim8_cc1,
  tim8_trgo,
  tim5_cc1,
  tim5_cc3
#endif
};

enum class inj_trig
{
  disable,

  tim1_trgo,
  tim1_cc4,

#if defined(ADC1)
  tim2_trgo,
  tim2_cc1,
  tim3_cc4,
  tim4_trgo,
  exti15__tim8_cc4,
#endif

  jsw_start,

#if defined(ADC3)
  tim4_cc3,
  tim8_cc2,
  tim8_cc4,
  tim5_trgo,
  tim5_cc4
#endif
};

enum class data_align { left, right };

enum class disc_mode { enable, disable, regular, injected, regular_and_injected };

enum class disc_num { _1ch, _2ch, _3ch, _4ch, _5ch, _6ch, _7ch, _8ch };

enum class dma { enable, disable };

enum class sample_time
{
  _1_5cyc,
  _7_5cyc,
  _13_5cyc,
  _28_5cyc,
  _41_5cyc,
  _55_5cyc,
  _71_5cyc,
  _239_5cyc
};

enum class conf { adc, reg_chan, inj_chan };

template<
  module_id _module_id,
  data_align _data_align,
  scan _scan,
  disc_mode _disc_mode,
  disc_num _disc_num = disc_num::_1ch,
  reg_trig _reg_trig = reg_trig::sw_start,
  inj_trig _inj_trig = inj_trig::disable,
  inj_auto _inj_auto = inj_auto::disable,
  dma _dma = dma::disable,
  dual_mode _dual_mode = dual_mode::independent,
  temp_sensor _temp_sensor = temp_sensor::disable
>
struct module
{
  static constexpr auto conf        = adc::conf::adc;
  static constexpr auto module_id   = _module_id;
  static constexpr auto data_align  = _data_align;
  static constexpr auto scan        = _scan;
  static constexpr auto disc_mode   = _disc_mode;
  static constexpr auto disc_num    = _disc_num;
  static constexpr auto reg_trig    = _reg_trig;
  static constexpr auto inj_trig    = _inj_trig;
  static constexpr auto inj_auto    = _inj_auto;
  static constexpr auto dma         = _dma;
  static constexpr auto dual_mode   = _dual_mode;
  static constexpr auto temp_sensor = _temp_sensor;
};

template<typename _module, uint8_t _chan_num, sample_time _sample_time>
struct channel_conf
{
  static constexpr auto conf        = adc::conf::reg_chan;
  static constexpr auto module      = _module();
  static constexpr auto chan_num    = _chan_num;
  static constexpr auto sample_time = _sample_time;
};

#include "detail/adc.h"

template<typename ...args>
void configure()
{
  detail::adc_configure<args...>();

#if defined(ADC1)
  detail::reg_chan_configure<module_id::adc1, args...>();
#endif

#if defined(ADC2)
  detail::reg_chan_configure<module_id::adc2, args...>();
#endif

#if defined(ADC3)
  detail::reg_chan_configure<module_id::adc3, args...>();
#endif

#if defined(ADC4)
  detail::reg_chan_configure<module_id::adc4, args...>();
#endif
}

template<typename _arg1, typename ...args>
void enable()
{
  detail::enable<_arg1>();
  if constexpr(sizeof...(args) > 0) { enable<args...>(); }
}

template<typename _arg1, typename ...args>
void disable()
{
  detail::disable<_arg1>();
  if constexpr(sizeof...(args) > 0) { disable<args...>(); }
}

template<typename ...args>
void calibrate() { detail::calibrate<args...>(); }

template<typename ...args>
void regular_soft_start() { detail::regular_soft_start<args...>(); }

template<typename _conf>
uint32_t read() { return detail::read<_conf>(); }

} // namespace adc
} // namespace lmcu
