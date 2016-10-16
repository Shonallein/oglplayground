#pragma once

namespace OglPlayground
{

// Imported from boost 
namespace noncopyable_  // protection from unintended ADL
{
  
class noncopyable
{
protected:
      noncopyable() = default;
      ~noncopyable() = default;
      noncopyable( const noncopyable& ) = delete;
      noncopyable& operator=( const noncopyable& ) = delete;
};

} // namespace noncopyable

typedef noncopyable_::noncopyable noncopyable;

} // namespace OglPlayground
