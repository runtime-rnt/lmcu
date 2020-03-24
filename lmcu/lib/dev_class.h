#pragma once

namespace lmcu {

enum class dev_class
{
  gpio_pin,
  timer,
  timer_in_channel,
  timer_out_channel,
  adc,
  adc_regular_channel,
  adc_injected_channel,
  i2c
};

} // namespace lmcu
