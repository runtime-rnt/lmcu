#pragma once

namespace lmcu {
namespace can {

enum class remap
{
  none = -1,
  CAN1__rx_PB8_tx_PB9 = AFIO_MAPR_CAN_REMAP_REMAP2
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

  static_assert(
    m.remap == remap::none ||
    m.remap == remap::CAN1__rx_PB8_tx_PB9,
    "invalid value of remap argument"
  );

  if constexpr(m.remap != remap::none) { return r | static_cast<uint32_t>(m.remap); }
  return r;
}

} // namespace detail

} // namespace can
} // namespace lmcu
