#include <vector>

#ifndef W_CLASS
#define W_CLASS

class W{
 public:
  
  W();
  W(unsigned int d);
  unsigned int& operator[](unsigned int i);
  void operator=(W obj);  
  bool is_null(void);
  void add_work(unsigned int delta, unsigned int sigma);
  void print_w(void);
  unsigned int get(unsigned int i);
  void add(unsigned int i, unsigned int e);
  void inc_time(unsigned int v);
  void set(unsigned int x, unsigned int e);
  void set_id(unsigned int k);
  unsigned int get_id(void);

 private:
  unsigned int m_id;
  std::vector<unsigned int> w;
};

std::vector<W> compute_w(unsigned int d, unsigned int s);
unsigned int state_space(unsigned int d, unsigned int s);

#endif
