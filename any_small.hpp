#pragma once

#include <type_traits>
#include <typeinfo>
#include <utility>
#include <stdexcept>

class any_small;

template<class ValueType>
const ValueType* any_cast(const any_small* operand);

template<class ValueType>
ValueType* any_cast(any_small* operand);

class any_small
{
  public:
    const static constexpr std::size_t max_size = 4 * sizeof(void*);

    any_small(const any_small& other)
    {
      other.get_abstract().copy_construct_into(get_abstract());
    }

    any_small(any_small&& other)
    {
      other.get_abstract().move_construct_into(get_abstract());
    }

    template<class T,
             class = typename std::enable_if<
               !std::is_same<
                 typename std::decay<T>::type, any_small
               >::value
             >::type,
             class = typename std::enable_if<
               sizeof(typename std::decay<T>::type) <= max_size
             >::type>
    any_small(T&& value)
    {
      new (&storage_) concrete<typename std::decay<T>::type>(std::forward<T>(value));
    }

    ~any_small()
    {
      get_abstract().~abstract();
    }

    const std::type_info& type() const
    {
      return get_abstract().type(); 
    }

    any_small& operator=(const any_small& other)
    {
      if(type() == other.type())
      {
        // the types match, just call the copy assign function
        other.get_abstract().copy_assign_to(get_abstract());
      }
      else
      {
        // the types match, need to destroy and then copy construct

        // destroy our value
        get_abstract().~abstract();

        // copy construct from the other value
        other.get_abstract().copy_construct_into(get_abstract());
      }

      return *this;
    }

    any_small& operator=(any_small&& other)
    {
      if(type() == other.type())
      {
        // the types match, just call the move assign function
        other.get_abstract().move_assign_to(get_abstract());
      }
      else
      {
        // the types match, need to destroy and then move construct

        // destroy our value
        get_abstract().~abstract();

        // move construct from the other value
        other.get_abstract().move_construct_into(get_abstract());
      }

      return *this;
    }

  private:
    template<class ValueType>
    friend const ValueType* any_cast(const any_small* operand);

    template<class ValueType>
    friend ValueType* any_cast(any_small* operand);

    struct abstract
    {
      virtual ~abstract(){}

      virtual void copy_construct_into(abstract& to) const = 0;
      virtual void move_construct_into(abstract& to) = 0;
      virtual void copy_assign_to(abstract& to) const = 0;
      virtual void move_assign_to(abstract& to) = 0;
      virtual const std::type_info& type() const = 0;
      virtual const void* ptr_to_value() const = 0;
      virtual void* ptr_to_value() = 0;
    };

    template<class T>
    struct concrete : abstract
    {
      template<class... Args>
      concrete(Args&&... arg) : value_(std::forward<Args>(arg)...) {}

      void copy_construct_into(abstract& to) const override
      {
        concrete& other = static_cast<concrete&>(to);

        new (&other) concrete(*this);
      }

      void move_construct_into(abstract& to) override
      {
        concrete& other = static_cast<concrete&>(to);

        new (&other) concrete(std::move(*this));
      }

      void copy_assign_to(abstract& to) const override
      {
        concrete& other = static_cast<concrete&>(to);

        other = *this;
      }

      void move_assign_to(abstract& to) override
      {
        concrete& other = static_cast<concrete&>(to);

        other = std::move(*this);
      }

      const std::type_info& type() const override
      {
        return typeid(T);
      }

      const void* ptr_to_value() const override
      {
        return &value_;
      }

      void* ptr_to_value() override
      {
        return &value_;
      }

      T value_;
    };

    abstract& get_abstract()
    {
      return *reinterpret_cast<abstract*>(&storage_);
    }

    const abstract& get_abstract() const
    {
      return *reinterpret_cast<const abstract*>(&storage_);
    }

    const void* ptr_to_value() const
    {
      return get_abstract().ptr_to_value();
    }

    void* ptr_to_value()
    {
      return get_abstract().ptr_to_value();
    }

    typename std::aligned_storage<4 * sizeof(void*)>::type storage_;
};


template<class ValueType>
const ValueType* any_cast(const any_small* operand)
{
  if(operand->type() != typeid(ValueType))
  {
    throw std::runtime_error("bad any_cast(const any_small*)");
  }

  return reinterpret_cast<const ValueType*>(operand->ptr_to_value());
}

template<class ValueType>
ValueType* any_cast(any_small* operand)
{
  if(operand->type() != typeid(ValueType))
  {
    throw std::runtime_error("bad any_cast(const any_small*)");
  }

  return reinterpret_cast<ValueType*>(operand->ptr_to_value());
}

template<class ValueType>
ValueType any_cast(const any_small& operand)
{
  return *any_cast<typename std::add_const<typename std::remove_reference<ValueType>::type>::type>(&operand);
}

template<class ValueType>
ValueType any_cast(any_small& operand)
{
  return *any_cast<typename std::remove_reference<ValueType>::type>(&operand);
}

template<class ValueType>
ValueType any_cast(any_small&& operand)
{
  return *any_cast<typename std::remove_reference<ValueType>::type>(&operand);
}

