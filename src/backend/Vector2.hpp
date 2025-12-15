#pragma once

#include <cstdint>
#include <initializer_list>
#include <ostream>

template<typename type = float>
struct Vector2 {
    type x;
    type y;

    /*######*/

    Vector2() {
        this->x = type(0);
        this->y = type(0);
    }

    Vector2(type x, type y) {
        this->x = type(x);
        this->y = type(y);
    }

    Vector2(std::initializer_list<type> list) {
        this->x = type(*(list.begin() + 0));
        this->y = type(*(list.begin() + 1));
    }

    Vector2(const Vector2<type> &vec) {
        this->x = type(vec.x);
        this->y = type(vec.y);
    }

    /*######*/

    /*######*/

    inline Vector2<type> get() { return *this; }

    inline Vector2<type> &link() { return *this; }

    inline void set(type x, type y) {
        this->x = type(x);
        this->y = type(y);
    }

    inline void operator=(Vector2<type> right) {
        this->x = right.x;
        this->y = right.y;
    }

    /*######*/

    template<typename old_type>
    static Vector2<type> to_type(std::initializer_list<old_type> list) {
        return Vector2<type>(type(static_cast<type>(*(list.begin() + 0))),
                             type(static_cast<type>(*(list.begin() + 1))));
    }
};

/* override */

// floting point
typedef Vector2<float> vec2f32;

typedef Vector2<double> vec2f64;

// signed
typedef Vector2<int8_t> vec2i8;

typedef Vector2<int16_t> vec2i16;

typedef Vector2<int32_t> vec2i32;

typedef Vector2<int64_t> vec2i64;

// unsigned
typedef Vector2<uint8_t> vec2ui8;

typedef Vector2<uint16_t> vec2ui16;

typedef Vector2<uint32_t> vec2ui32;

typedef Vector2<uint64_t> vec2ui64;

/* +++ */
template<typename type>
inline Vector2<type> operator+(const Vector2<type> &left,
                               const Vector2<type> &right) {
    return Vector2<type>(left.x + right.x, left.y + right.y);
}

template<typename type>
inline void operator+=(Vector2<type> &left, const Vector2<type> &right) {
    left.x += right.x;
    left.y += right.y;
}

/* --- */
template<typename type>
inline Vector2<type> operator-(const Vector2<type> &left,
                               const Vector2<type> &right) {
    return Vector2<type>(left.x - right.x, left.y - right.y);
}

template<typename type>
inline void operator-=(Vector2<type> &left, const Vector2<type> &right) {
    left.x -= right.x;
    left.y -= right.y;
}

/* *** */

template<typename type>
inline Vector2<type> operator*(const Vector2<type> &left,
                               const Vector2<type> &right) {
    return Vector2<type>(left.x * right.x, left.y * right.y);
}

template<typename type>
inline void operator*=(Vector2<type> &left, const Vector2<type> &right) {
    left.x *= right.x;
    left.y *= right.y;
}

/* /// */

template<typename type>
inline Vector2<type> operator/(const Vector2<type> &left,
                               const Vector2<type> &right) {
    return Vector2<type>(left.x / right.x, left.y / right.y);
}

template<typename type>
inline void operator/=(Vector2<type> &left, const Vector2<type> &right) {
    left.x /= right.x;
    left.y /= right.y;
}

/*######*/
/*######*/

/* BOOLEN */

template<typename type>
inline bool operator==(const Vector2<type> &left, const Vector2<type> &right) {
    return (left.x == right.x) && (left.y == right.y);
}

template<typename type>
inline bool operator!=(const Vector2<type> &left, const Vector2<type> &right) {
    return !(left == right);
}

/* >>> */

template<typename type>
inline bool operator>(const Vector2<type> &left, const Vector2<type> &right) {
    if (left.x != right.x) return left.x > right.x;
    return left.y > right.y;
}

template<typename type>
inline bool operator>=(const Vector2<type> &left, const Vector2<type> &right) {
    return !(left < right);
}

/* <<< */

template<typename type>
inline bool operator<(const Vector2<type> &left, const Vector2<type> &right) {
    if (left.x != right.x) return left.x < right.x;
    return left.y < right.y;
}

template<typename type>
inline bool operator<=(const Vector2<type> &left, const Vector2<type> &right) {
    return !(left > right);
}

/* <<< output stream operator */
template<typename type>
inline std::ostream &operator<<(std::ostream &os, const Vector2<type> &vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}