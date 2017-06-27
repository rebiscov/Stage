#include <vector>

#ifndef W_CLASS
#define W_CLASS

class W{
 public:
  
  W();
  W(unsigned int d);
  unsigned int& operator[](unsigned int i);
  void operator=(W obj);
  bool operator==(const W& obj) const;  
  bool operator<(const W& obj) const;
  bool is_null(void);
  void add_work(unsigned int delta, unsigned int sigma);
  void print_w(void);
  unsigned int get(unsigned int i) const;
  unsigned int size(void) const;
  void add(unsigned int i, unsigned int e);
  void inc_time(unsigned int v);
  void set(unsigned int x, unsigned int e);

 private:
  unsigned int* w;
  unsigned int m_d;
};

std::vector<W> compute_w(unsigned int d, unsigned int s);
unsigned int state_space(unsigned int d, unsigned int s);

namespace std {
  template <>
  struct hash<W>
  {
    std::size_t operator()(const W& obj) const
    {
      unsigned int hash = 0;
      for (unsigned int i = 1; i <= obj.size(); i++){
	hash *= 37;
	hash += obj.get(i);
      }
      return (std::size_t)hash;
    }
  };
}

#endif
