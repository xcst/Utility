#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>
#include <format>
#include <initializer_list>
#include <iterator>

template<typename T, size_t N>
struct static_vector
{
    using value_type = T;
    using size_type = typename std::array<T, N>::size_type;
    using difference_type = typename std::array<T, N>::difference_type;
    using reference = typename std::array<T, N>::reference;
    using const_reference = typename std::array<T, N>::const_reference;
    using pointer = typename std::array<T, N>::pointer;
    using const_pointer = typename std::array<T, N>::const_pointer;
    using iterator = typename std::array<T, N>::iterator;
    using const_iterator = typename std::array<T, N>::const_iterator;
    using reverse_iterator = typename std::array<T, N>::reverse_iterator;
    using const_reverse_iterator = typename std::array<T, N>::const_reverse_iterator;

    std::array<T, N> m_data;
    iterator m_end = m_data.begin();

    constexpr static_vector() noexcept = default;
    constexpr static_vector(size_type count, const T& value)
    {
        if (count > N)
            throw std::out_of_range("static_vector cannot be constructed beyond its size");

        std::fill(m_data.begin(), m_data.end(), value);
        m_end += count;
    }
    constexpr explicit static_vector(size_type count)
        : static_vector(count, T())
    {
    }
    template<class InputIt>
    constexpr static_vector(InputIt first, InputIt last)
    {
        if (std::distance(first, last) > N)
            throw std::out_of_range("static_vector cannot be constructed beyond its size");

        std::copy(first, last, m_data.begin());
        m_end += std::distance(first, last);
    }
    constexpr static_vector(const static_vector& other) = default;
    constexpr static_vector(static_vector&& other) noexcept = default;
    constexpr static_vector(std::initializer_list<T> init)
    {
        if (init.size() > N)
            throw std::out_of_range("static_vector cannot be constructed beyond its size");

        std::copy(init.begin(), init.end(), m_data.begin());
        m_end += init.size();
    }

    constexpr reference at(size_type pos)
    {
        if (pos >= size())
            throw std::out_of_range(std::format("Out of range access (pos = {}, N = {})", pos, N));

        return m_data[pos];
    }
    constexpr const_reference at(size_type pos) const
    {
        if (pos >= size())
            throw std::out_of_range(std::format("Out of range access (pos = {}, N = {})", pos, N));

        return m_data[pos];
    }

    constexpr reference operator[](size_type pos)
    {
        return m_data[pos];
    }
    constexpr const_reference operator[](size_type pos) const
    {
        return m_data[pos];
    }

    constexpr reference front()
    {
        return m_data.front();
    }
    constexpr const_reference front() const
    {
        return m_data.front();
    }

    constexpr reference back()
    {
        auto tmp = end();
        --tmp;
        return *tmp;
    }
    constexpr const_reference back() const
    {
        auto tmp = cend();
        --tmp;
        return *tmp;
    }

    constexpr pointer data() noexcept
    {
        return m_data.data();
    }
    constexpr const_pointer data() const noexcept
    {
        return m_data.data();
    }

    constexpr iterator begin() noexcept
    {
        return m_data.begin();
    }
    constexpr const_iterator begin() const noexcept
    {
        return m_data.begin();
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }

    constexpr iterator end() noexcept
    {
        return m_end;
    }
    constexpr const_iterator end() const noexcept
    {
        return m_end;
    }
    constexpr const_iterator cend() const noexcept
    {
        return m_end;
    }

    constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(m_end - 1);
    }
    constexpr const_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(m_end - 1);
    }
    constexpr const_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(m_end - 1);
    }

    constexpr reverse_iterator rend() noexcept
    {
        return m_data.rend();
    }
    constexpr const_iterator rend() const noexcept
    {
        return m_data.rend();
    }
    constexpr const_iterator crend() const noexcept
    {
        return m_data.crend();
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return m_data.begin() == m_end;
    }

    constexpr size_type size() const noexcept
    {
        return std::distance(begin(), end());
    }

    constexpr size_type max_size() const noexcept
    {
        return N;
    }

    constexpr void clear() noexcept
    {
        m_end = m_data.begin();
    }

    constexpr iterator insert(const_iterator pos, const T& value);
    constexpr iterator insert(const_iterator pos, T&& value);
    constexpr iterator insert(const_iterator pos, size_type count, const T& value);
    template<class InputIt>
    constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);
    constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);

    constexpr iterator erase(const_iterator pos);
    constexpr iterator erase(const_iterator first, const_iterator last);

    constexpr void push_back(const T& value)
    {
        if (size() == N)
            throw std::out_of_range("static_vector cannot be grown beyond its size");

        *(m_end++) = value;
    }
    constexpr void push_back(T&& value)
    {
        if (size() == N)
            throw std::out_of_range("static_vector cannot be grown beyond its size");

        *(m_end++) = value;
    }

    template<class... Args>
    constexpr reference emplace_back(Args&&... args)
    {
        if (size() == N)
            throw std::out_of_range("static_vector cannot be grown beyond its size");

        *(m_end++) = T(std::forward<Args>(args)...);
    }

    constexpr void pop_back()
    {
        if (size() > 0)
            m_end--;
    }

    constexpr void resize(size_type count)
    {
        if (count >= N)
            throw std::out_of_range("static_vector cannot be grown beyond its size");

        if (count == size())
            return;

        if (count > size())
        {
            std::fill(m_end, m_data.begin() + count, T());
        }
        m_end = m_data().begin() + count;
    }
    constexpr void resize(size_type count, const value_type& value)
    {
        if (count >= N)
            throw std::out_of_range("static_vector cannot be grown beyond its size");

        if (count == size())
            return;

        if (count > size())
        {
            std::fill(m_end, m_data.begin() + count, value);
        }
        m_end = m_data().begin() + count;
    }

    // TODO: copy, move constructors
};
