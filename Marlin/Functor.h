#ifndef FUNCTOR_H
#define FUNCTOR_H

template <typename R, typename... Args>
class Functor
{
   public:
      typedef R (*FuncPtr)(Args... args);

   public:
      Functor (FuncPtr fptr)
         : m_fptr(fptr)
      {};

      R action(Args... args)
      {
         return m_fptr(args...);
      };

   private:
      FuncPtr m_fptr;
};

#endif //FUNCTOR_H
