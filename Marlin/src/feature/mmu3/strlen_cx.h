#pragma once

constexpr inline int strlen_constexpr(const char *str) {
    return *str ? 1 + strlen_constexpr(str + 1) : 0;
}
