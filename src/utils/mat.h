#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>

template <size_t rowCount, size_t colCount>
struct MatTraits {
    constexpr static bool is_row_vec{false};
    constexpr static bool is_col_vec{false};
    constexpr static bool is_scalar{false};
    constexpr static bool is_vec{false};
    constexpr static size_t dim{rowCount * colCount};
};

template <>
struct MatTraits<1, 1> {
    constexpr static bool is_row_vec{false};
    constexpr static bool is_col_vec{false};
    constexpr static bool is_scalar{true};
    constexpr static bool is_vec{false};
    constexpr static size_t dim{1};
};

template <size_t rowCount>
struct MatTraits<rowCount, 1> {
    constexpr static bool is_row_vec{false};
    constexpr static bool is_col_vec{true};
    constexpr static bool is_scalar{false};
    constexpr static bool is_vec{true};
    constexpr static size_t dim{rowCount};
};

template <size_t colCount>
struct MatTraits<1, colCount> {
    constexpr static bool is_row_vec{true};
    constexpr static bool is_col_vec{false};
    constexpr static bool is_scalar{false};
    constexpr static bool is_vec{true};
    constexpr static size_t dim{colCount};
};

template <size_t R, size_t C>
class MatX {
  public:
    // ------------------------------------
    // -------------- Traits --------------
    // ------------------------------------
    using Traits = MatTraits<R, C>;
    constexpr static bool is_row_vec{Traits::is_row_vec};
    constexpr static bool is_col_vec{Traits::is_row_vec};
    constexpr static bool is_vec{Traits::is_vec};
    constexpr static bool is_scalar{Traits::is_scalar};
    constexpr static size_t dim{Traits::dim};

    constexpr static size_t row_count() { return R; }
    constexpr static size_t col_count() { return C; }

    // ---------------------------------------------
    // -------------- Common matrices --------------
    // ---------------------------------------------
    static MatX all(double x) {
        std::array<double, dim> m;
        std::fill(m.begin(), m.end(), x);
        return m;
    }

    static MatX zero() { return MatX::all(0.0); }

    static MatX one() { return MatX::all(1.0); }

    template <bool cond = (R == C), typename = std::enable_if_t<cond>>
    static MatX diagonal(double x) {
        auto res{MatX::zero()};
        for (size_t i{0}; i < R; ++i) {
            res(i, i) = x;
        }
        return res;
    }

    template <bool cond = (R == C), typename = std::enable_if_t<cond>>
    static MatX identity() {
        return diagonal(1.0);
    }

    // -----------------------------------------------------
    // -------------- Constructors/Destructor --------------
    // -----------------------------------------------------
    MatX() = default;

    template <typename... Ts>
    MatX(Ts... args) {
        if constexpr (sizeof...(args)) {
            size_t i{};
            auto y = {(m[i++] = args, 0)...};
        }
    }

    MatX(std::initializer_list<std::initializer_list<double>> il) {
        size_t i{};
        for (auto&& row : il) {
            for (auto&& x : row) {
                m[i++] = x;
            }
        }
    }

    MatX(std::array<double, R * C> arr) : m{arr} {}

    template <bool cond = is_scalar, typename = std::enable_if_t<cond>>
    operator double() const {
        return m[0];
    }

    MatX(const MatX&) = default;
    MatX(MatX&&) noexcept = default;
    MatX& operator=(const MatX&) = default;
    MatX& operator=(MatX&&) noexcept = default;
    ~MatX() = default;

    // ---------------------------------------
    // -------------- Operators --------------
    // ---------------------------------------
    double& operator[](size_t i) { return m[i]; }
    const double& operator[](size_t i) const { return m[i]; }
    double& operator()(size_t i, size_t j) { return m[i * C + j]; }
    const double& operator()(size_t i, size_t j) const { return m[i * C + j]; }

    MatX operator-() const {
        decltype(m) n{m};
        std::for_each(n.begin(), n.end(), [](double& x) { x = -x; });
        return n;
    }

    MatX& operator+=(const MatX& rhs) {
        for (size_t i{}; i < dim; ++i) {
            m[i] += rhs[i];
        }
        return *this;
    }

    MatX& operator-=(const MatX& rhs) { return *this += (-rhs); }

    MatX& operator*=(double x) {
        for (size_t i{}; i < dim; ++i) {
            m[i] *= x;
        }
        return *this;
    }

    MatX& operator/=(double x) { return (*this) *= (1 / x); }

    // -----------------------------------------------------
    // -------------- Matrix/Vector utilities --------------
    // -----------------------------------------------------
    MatX<1, C> row(size_t i) const {
        MatX<1, C> res{};
        for (size_t j{}; j < C; ++j) {
            res[j] = (*this)(i, j);
        }
        return res;
    }

    MatX<R, 1> col(size_t j) const {
        MatX<R, 1> res{};
        for (size_t i{}; i < R; ++i) {
            res[i] = (*this)(i, j);
        }
        return res;
    }

    template <bool cond = is_vec, typename = std::enable_if_t<cond>>
    double& x() {
        return m[0];
    }

    template <bool cond = is_vec, typename = std::enable_if_t<cond>>
    const double& x() const {
        return m[0];
    }

    template <bool cond = is_vec && (dim > 1), typename = std::enable_if_t<cond>>
    double& y() {
        return m[1];
    }

    template <bool cond = is_vec && (dim > 1), typename = std::enable_if_t<cond>>
    const double& y() const {
        return m[1];
    }

    template <bool cond = is_vec && (dim > 2), typename = std::enable_if_t<cond>>
    double& z() {
        return m[2];
    }

    template <bool cond = is_vec && (dim > 2), typename = std::enable_if_t<cond>>
    const double& z() const {
        return m[2];
    }

    template <bool cond = is_vec && (dim > 3), typename = std::enable_if_t<cond>>
    double& w() {
        return m[3];
    }

    template <bool cond = is_vec && (dim > 3), typename = std::enable_if_t<cond>>
    const double& w() const {
        return m[3];
    }

    template <bool cond = (R == C), typename = std::enable_if_t<cond>>
    double det() const {
        if constexpr (R == 1) {
            return m[0];
        } else {
            double res{};
            for (size_t j{}; j < C; ++j) {
                // NOTE: Precedence of % and ?
                double sign{((j % 2) ? -1.0 : 1.0)};
                MatX<R - 1, C - 1> minor_mat{minor(0, j)};
                res += sign * m[j] * minor_mat.det();
            }
            return res;
        }
    }

    template <bool cond = (R == C), typename = std::enable_if_t<cond>>
    MatX inverse() const {
        MatX<R, C> cofactor{zero()};
        double d{det()};
        if (d == 0.0) {
            return cofactor;
        }

        for (size_t i{}; i < R; ++i) {
            for (size_t j{}; j < C; ++j) {
                auto m = minor(i, j);
                auto d = m.det();
                // NOTE: Precedence! (i + j) % 2
                double sign{((i + j) % 2 ? -1.0 : 1.0)};
                cofactor(i, j) = sign * minor(i, j).det();
            }
        }

        return cofactor.transposed() / d;
    }

    MatX<R - 1, C - 1> minor(size_t i, size_t j) const {
        size_t idx{};
        MatX<R - 1, C - 1> res;
        for (size_t r{}; r < R; ++r) {
            for (size_t c{}; c < C; ++c) {
                if (r == i || c == j) {
                    continue;
                }
                res[idx++] = (*this)(r, c);
            }
        }
        return res;
    }

    MatX<C, R> transposed() const {
        MatX<C, R> res;
        for (size_t i{}; i < R; ++i) {
            for (size_t j{}; j < C; ++j) {
                res(j, i) = (*this)(i, j);
            }
        }
        return res;
    }

    template <bool cond = is_vec, typename = std::enable_if_t<cond>>
    double norm() const {
        double res{};
        for (size_t i{}; i < dim; ++i) {
            res += m[i] * m[i];
        }
        return std::sqrt(res);
    }

    template <bool cond = is_vec, typename = std::enable_if_t<cond>>
    MatX normalized() const {
        if (norm() == 0.0) {
            return (*this);
        }
        return (*this) * (1 / norm());
    }

    template <bool cond = is_vec, typename = std::enable_if_t<cond>>
    void normalize() {
        (*this) /= norm();
    }

    std::array<double, R * C> get_array() const { return m; }

  private:
    std::array<double, R * C> m;
};

template <size_t R, size_t C>
bool operator==(const MatX<R, C>& m, const MatX<R, C>& n) {
    return m.get_array() == n.get_array();
}

template <size_t R, size_t C>
bool operator!=(const MatX<R, C>& m, const MatX<R, C>& n) {
    return !(m == n);
}

template <size_t R, size_t C>
MatX<R, C> operator+(const MatX<R, C>& m, const MatX<R, C>& n) {
    MatX<R, C> res{m};
    return res += n;
}

template <size_t R, size_t C>
MatX<R, C> operator-(const MatX<R, C>& m, const MatX<R, C>& n) {
    MatX<R, C> res{m};
    return res -= n;
}

template <size_t R, size_t C>
MatX<R, C> operator*(const MatX<R, C>& m, double x) {
    MatX<R, C> res{m};
    return res *= x;
}

template <size_t R, size_t C>
MatX<R, C> operator/(const MatX<R, C>& m, double x) {
    MatX<R, C> res{m};
    return res /= x;
}

template <size_t R, size_t C>
MatX<R, C> operator/(const MatX<R, C>& m, const MatX<R, C>& n) {
    MatX<R, C> res{m};
    for (size_t i{}; i < R * C; ++i) {
        res[i] /= n[i];
    }
    return res;
}

template <size_t R, size_t C>
MatX<R, C> operator*(double x, const MatX<R, C>& m) {
    return m * x;
}

template <size_t L>
MatX<1, 1> operator*(const MatX<1, L>& m, const MatX<L, 1>& n) {
    double sum{};
    for (size_t i{}; i < L; ++i) {
        sum += m[i] * n[i];
    }
    return sum;
}

template <size_t M, size_t N, size_t P>
MatX<M, P> operator*(const MatX<M, N>& m, const MatX<N, P>& n) {
    MatX<M, P> res;
    for (size_t i{}; i < M; ++i) {
        for (size_t j{}; j < P; ++j) {
            res(i, j) = m.row(i) * n.col(j);
        }
    }
    return res;
}

template <size_t R, size_t C>
MatX<R, C> hadamard(const MatX<R, C>& m, const MatX<R, C>& n) {
    MatX<R, C> res{m};
    for (size_t i{}; i < R * C; ++i) {
        res[i] *= n[i];
    }
    return res;
}

template <size_t R, size_t C>
MatX<R, C> transposed(const MatX<R, C>& m) {
    return m.transposed();
}

template <size_t R, size_t C>
bool near_equal(const MatX<R, C>& m, const MatX<R, C>& n, double tolerance = 0.005) {
    auto arrm{m.get_array()};
    auto arrn{n.get_array()};
    for (size_t i{}; i < arrm.size(); ++i) {
        if (std::abs(arrm[i] - arrn[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

template <size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const MatX<R, C>& mat) {
    for (size_t i{}; i < R; ++i) {
        for (size_t j{}; j < C - 1; ++j) {
            printf("%7.4f\t", mat(i, j));
        }
        printf("%7.4f\n", mat(i, C - 1));
    }
    return os;
}

using Mat2 = MatX<2, 2>;
using Mat3 = MatX<3, 3>;
using Mat4 = MatX<4, 4>;
