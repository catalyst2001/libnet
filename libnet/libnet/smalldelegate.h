/**
* Small Delegates
* 
* Date: 19.01.2025 
* Author: Deryabin K.
*/
#pragma once
#include <array>
#include <vector>
#include <algorithm>

/**
* small_sc_delegate
* single cast delegate class
*/
template<typename _ret_type, class _class, typename... _args>
class small_sc_delegate {
public:
  using _thisclass = small_sc_delegate<_ret_type, _class, _args...>;
  using _methodptr = _ret_type(_class::*)(_args...);
private:
  _methodptr m_pmethod;
  _class* m_pthis;
public:
  small_sc_delegate() : m_pmethod(nullptr), m_pthis(nullptr) {}
  small_sc_delegate(_methodptr pmethod) : m_pmethod(pmethod), m_pthis(nullptr) {}
  small_sc_delegate(_class* pthis, _methodptr pmethod) : m_pmethod(pmethod), m_pthis(pthis) {}
  /* checking for valid */
  inline bool is_valid() { return m_pmethod && m_pthis; }
  inline operator bool() { return is_valid(); }
  /* bindings */
  void set_method(_methodptr pmethod) { m_pmethod = pmethod; }
  void bind(_class* thisptr) { m_pthis = thisptr; }
  void bind(_class* thisptr, _methodptr pmethod) {
    m_pthis = thisptr;
    m_pmethod = pmethod;
  }
  /* functor */
  _ret_type operator()(_args... args) {
    return (m_pthis->*m_pmethod)(args...);
  }
  /* compare */
  inline bool compare_to(_thisclass& _other_delegate) {
    return m_pmethod == _other_delegate.m_pmethod && m_pthis == _other_delegate.m_pthis;
  }
  inline bool operator==(_thisclass& _other_delegate) { return compare_to(_other_delegate); }
  inline bool operator!=(_thisclass& _other_delegate) { return !compare_to(_other_delegate); }
};

/**
* small_mc_delegate
* thread unsafe dynamic multicast delegate
*/
template<typename _ret_type, class _class, typename... _args>
class small_mc_delegate
{
public:
  using _deleate_type = small_sc_delegate<_ret_type, _class, _args...>;
private:
  std::vector<_deleate_type> m_delegates;
public:
  small_mc_delegate() {}
  inline bool is_empty() { return m_delegates.empty(); }
  inline _deleate_type& gelegate_at(size_t _idx) { return m_delegates[_idx]; }
  inline bool add(_deleate_type _new_delegate) {
    try {
      m_delegates.push_back(_new_delegate);
    }
    catch (...) {
      return false;
    }
    return true;
  }

  inline bool remove(_deleate_type _delegate_for_delete) {
    if (m_delegates.empty())
      return false;
    auto it = std::find(m_delegates.begin(), m_delegates.end(), _delegate_for_delete);
    if (it != m_delegates.end()) {
      /* invalidate delegate */
      it.bind(nullptr, nullptr);
      m_delegates.erase(it);
      return true;
    }
    /* not found delegate */
    return false;
  }
  /* add/remove delegates */
  inline bool operator+=(_deleate_type _new_delegate) { return add(_new_delegate); }
  inline bool operator-=(_deleate_type _new_delegate) { return remove(_new_delegate); }
  inline _deleate_type& operator[](size_t _idx) { return m_delegates[_idx]; }

  /* functor */
  _ret_type operator()(_args... args) {
    _ret_type _ret_value = _ret_type();
    for (auto& delegate : m_delegates) {
      _ret_value = delegate(args...);
    }
    return _ret_value;
  }
};

#if 0
void main()
{
  class test {
  public:
    int c;
    test() : c(0) {}
    int method1(int a, float b) {
      c = a + b;
      printf("method1:  a=%d + b=%f = %d\n", a, b, c);
      return c;
    }
    int method2(int a, float b) {
      c = a + b;
      printf("method2:  a=%d + b=%f = %d\n", a, b, c);
      return c;
    }
    void method3(int a, float b) {
      c = a + b;
      printf("method3:  a=%d + b=%f = %d\n", a, b, c);
    }
  };

  /* test delegate 1 */
  printf("small singlecast delegate test\n");
  test object1;
  small_sc_delegate<int, test, int, float> delegate1(&object1, &test::method1);
  delegate1.bind(&object1);
  printf("%d\n", delegate1(5, 5));

  test object2;
  small_sc_delegate<void, test, int, float> delegate2(&object2, &test::method3);
  delegate2.bind(&object2);
  delegate2(1, 1);
  printf("%d\n", object2.c);

  /* multicast delegates test */
  small_mc_delegate<int, test, int, float> multicast_delegate;
  multicast_delegate += small_sc_delegate<int, test, int, float>(&object2, &test::method1);
  multicast_delegate += small_sc_delegate<int, test, int, float>(&object2, &test::method2);

  multicast_delegate[0](5, 10);
  multicast_delegate[1](20, 10);

  printf("call muiticast delegate\n");
  int value = multicast_delegate(1, 6.f);
}
#endif