#include <iostream>
#include <cmath>
#include <string>
#include <stdlib.h>

using namespace std;

class Rational {
public: 
  Rational(int number1, int number2);
  Rational(int number1);
  Rational();
  /*
  Rational input(istream& in_stream);  
  */
  friend Rational output(ostream &out_stream, Rational num);

  Rational add(Rational num);
  Rational sub(Rational num);
  Rational mul (Rational num);
  Rational div(Rational num);
  Rational neg();
  bool less(Rational num);
public:
  int numer, denom;
};

Rational::Rational(int number1, int number2) {
 this->numer = number1;
 this->denom = number2;
}

Rational::Rational(int number1) {
 this->numer = number1;
 this->denom = 1; 
}

Rational::Rational() {
  numer = 0;
  denom = 0;
}

ostream &operator<<(ostream &out_stream, Rational num) {
  out_stream << num.numer << '/' << num.denom << endl;
  return out_stream;
}

//istream &operator>>(istream &in_stream, Rational num) {
  

Rational Rational::add(Rational num) {
  Rational newRat((this->numer * num.denom) + (this->denom * num.numer),
		  (this->denom * num.denom));
  return newRat;
}

Rational Rational::sub(Rational num) {
  Rational newRat((this->numer * num.denom) - (this->denom * num.numer),
		  (this->denom * num.denom));
  return newRat;
}

Rational Rational::mul(Rational num) {
  Rational newRat((this->numer * num.numer) / (this->denom * num.denom));
  return newRat;
}

Rational Rational::div(Rational num) {
  Rational newRat((this->numer * num.denom) / (num.numer * this->denom));
  return newRat;
}

Rational Rational::neg() {
  Rational newRat(-(this->numer), this->denom);
  return newRat;
}

bool Rational::less(Rational num) {
  return ((this->numer * num.denom) / (num.numer * this->denom));
}

int main() {
  Rational myRat(4, 5);

  Rational myRat = 1/2;

  cout << myRat;

  Rational rat(4, 5);

  Rational frac = myRat.add(rat);
  cout << myRat;
    
  return 0;
}
