template<typename module_t>
inline uint32_t one_cycle_delay()
{
  return rcc::system_clock() / (rcc::apb2_clock() / uint32_t(module_t().baud_prediv));
}

template<typename module_t>
uint16_t rx()
{
  constexpr auto m = module_t();
  auto inst = detail::inst<m.module_id>();

  if constexpr(m.mode == mode::master) {
    // one SPI cycle in system clocks
    const uint32_t delay_val = one_cycle_delay<module_t>();
    if constexpr(m.crc == crc::enable) {
      if(delay_val <= 4) { return master_rx_with_crc(inst); }
      return master_rx_with_crc(inst, delay_val);
    }
    if(delay_val <= 4) { return master_rx(inst); }
    return master_rx(inst, delay_val);
  }

  if constexpr(m.crc == crc::enable) { return slave_rx_with_crc(inst); }
  return slave_rx(inst);
}

template<typename module_t, typename data_t>
void read(data_t *data, uint32_t count)
{
  constexpr auto m = module_t();
  auto inst = detail::inst<m.module_id>();

  if constexpr(m.mode == mode::master) {
    // one SPI cycle in system clocks
    const uint32_t delay_val = one_cycle_delay<module_t>();
    if constexpr(m.crc == crc::enable) {
      if(delay_val <= 4) { master_read_with_crc(inst, data, count); }
      else { master_read_with_crc(inst, data, count, delay_val); }
    }
    else {
      if(delay_val <= 4) { master_read(inst, data, count); }
      else { master_read(inst, data, count, delay_val); }
    }
  }
  else {
    if constexpr(m.crc == crc::enable) { slave_read_with_crc(inst, data, count); }
    else { slave_read(inst, data, count); }
  }
}
