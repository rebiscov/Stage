#include <iostream>
#include <vector>
#include "math.hpp"
#include "states.hpp"

W::W(void){
}

W::W(unsigned int d): w(std::vector<unsigned int>(d,0)){
}

unsigned int& W::operator[](unsigned int i){
  return w[i];
}

void W::operator=(W obj){
  w = obj.w;
}

bool W::operator<(const W& obj) const{
  unsigned int d = w.size();
  for (unsigned int i = 0; i < d; i++)
    if (w[i] != obj.w[i])
      return w[i] < obj.w[i];
  return w[d-1] < obj.w[d-1];
}

bool W::is_null(void){
  for (unsigned int i = 0; i < w.size(); i++)
    if (w[i] != 0)
      return false;
  return true;
}

void W::add_work(unsigned int delta, unsigned int sigma){
  for (unsigned int i = delta-1; i < w.size(); i++)
    w[i] += sigma;
}


void W::print_w(void){
  for (unsigned int i = 0; i < w.size()-1; i++)
    std::cout << w[i] << " ";
  std::cout << w.back() << std::endl;
}

unsigned int W::get(unsigned int i){
  if (i == 0)
    return 0;
  else
    return w[i-1];
}

void W::add(unsigned int i, unsigned int e){
  unsigned int d = w.size();

  for (unsigned int j = i-1; j < d; j++)
    w[j] += e;
}

void W::inc_time(unsigned int v){
  unsigned int d = w.size();
  for (unsigned int i = 0; i < d-1; i++)
    w[i] = max(w[i+1]-v, 0);
  w[d-1] = w[d-2];
}

void W::set(unsigned int x, unsigned int e){
  unsigned int d = w.size();

  w[x-1] = e;
  if ((x < d && w[x] < w[x-1]) || (x > 1 && w[x-2] > w[x-1])){
    std::cout << "States.set: not a valid value, this is not a valid remaining function anymore" << std::endl;
  }
}

unsigned int state_space(unsigned int d, unsigned int s){
  return c(d,s);
}


void compute_w_i(std::vector<W>& w_set, W w, unsigned int d, unsigned int s, unsigned int sigma_i, unsigned int i, unsigned int &index){
  if (i >= d){
    w_set[index] = w;
    index++;
  }
  else
    for (unsigned int sigma = sigma_i; sigma <= (i+1)*s; sigma++){
      w[d-i-1] = sigma;
      compute_w_i(w_set, w, d, s, sigma, (i+1), index);
    }
}

std::vector<W> compute_w(unsigned int d, unsigned int s){
  unsigned int space = state_space(d, s), index = 0;
  std::vector<W> w_set(space);
  W w(d);

  compute_w_i(w_set, w, d, s, 0, 0, index);

  for (unsigned int i = 0; i < space; i++){
    w_set[i][0] -= w_set[i][1];
    for (unsigned int j = 1; j < d-1; j++)
      w_set[i][j] += w_set[i][j-1] - w_set[i][j+1];
    w_set[i][d-1] += w_set[i][d-2];
  }

  return w_set;
}


