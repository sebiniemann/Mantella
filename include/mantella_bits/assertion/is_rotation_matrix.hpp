#pragma once

template <typename T, std::size_t N>
bool is_rotation_matrix(
    const typename std::array<T, N*N>::const_iterator matrix);
    
//
// Implementation
//