#include <iostream>
#include <cmath>
#include <string>
#include <stdlib.h>

namespace grpatter{
using namespace std;

class Rational {
public: 
  Rational(int number1, int number2);
  Rational(int whole_number);
  Rational();
  friend Rational input(istream& in_stream);  
  friend Rational output(ostream &out_stream, Rational num);
  friend bool &operator==(Rational num1, Rational num2);
  friend bool &operator<(Rational num1, Rational num2);
  friend bool &operator<=(Rational num1, Rational num2);
  friend bool &operator>(Rational num1, Rational num2);
  friend bool &operator>=(Rational num1, Rational num2);
  friend Rational &operator+(Rational num1, Rational num2);
  friend Rational &operator-(Rational num1, Rational num2);
  friend Rational &operator*(Rational num1, Rational num2);
  friend Rational &operator/(Rational num1, Rational num2);
  Rational fracReduce(Rational num);
  Rational fracNorm(Rational num);
public:
  int numer, denom;
};

Rational::Rational(int number1, int number2) {
 this->numer = number1;
 this->denom = number2;
}

Rational::Rational(int whole_number) {
 this->numer = whole_number;
 this->denom = 1; 
}

Rational::Rational() {
  numer = 0;
  denom = 1;
}

ostream &operator<<(ostream &out_stream, Rational num) {
  out_stream << num.numer << '/' << num.denom << endl;
  return out_stream;
}

Rational &operator>>(istream &in_stream, Rational num){
  cout << "Enter Numerator: " << endl;
  in_stream >> num.numer;
  cout << "Enter Denominator: " << endl;
  int_stream >> num.denom;
  Rational newRat(num.numer, num.denom);
  return newFrac(newRat);
}
  
bool &operator==(Rational num1, Rational num2) {
  if ((num1.numer * num2.denom) == (num2.numer * num1.denom)) {
    return true;
  }
}

bool &operator<(Rational num1, Rational num2) {
  if ((num1.numer * num2.denom) < (num2.numer * num1.denom)){
    return true;
  }
}

bool &operator<=(Rational num1, Rational num2) {
  if (((num1.numer * num2.denom) == (num2.numer * num1.denom)) ||
      ((num1.numer * num2.denom) < (num2.numer * num1.denom))){
    return true;
  }
}

bool &operator>(Rational num1, Rational num2) {
  if ((num1.numer * num2.denom) > (num2.numer * num1.denom)){
  return true;
  }
}

bool &operator>=(Rational num1, Rational num2) {
  if (((num1.numer * num2.denom) == (num2.numer * num1.denom)) ||
      ((num1.numer * num2.denom) > (num2.numer * num1.denom))) {
    return true;
  }
}

Rational &operator+(Rational num1, Rational num2) {
  Rational newRat(((num1.numer * num2.denom) + (num1.denom * num2.numer)),
		  (num1.denom * num2.denom));
  return newRat;
}

Rational &operator-(Rational num1, Rational num2) {
  Rational newRat(((num1.numer * num2.denom) - (num1.denom * num2.numer)),
		  (num1.denom * num2.denom));
  return newRat;
}

Rational &operator*(Rational num1, Rational num2) {
  Rational newRat((num1.numer * num2.numer), (num1.denom * num2.denom));
  return newRat;
}

Rational &operator/(Rational num1, Rational num2) {
  Rational newRat((num1.numer * num2.denom), (num2.numer * num1.denom));
  return newRat;
}

Rational Rational::fracReduce(Rational num) {
  if ((int == (num.numer / 2)) && (int == (num.denom / 2))) {
    Rational newFrac ((num.numer / 2), (num.denom / 2));
    fracReduce(newFrac);
  } else
    return num;
}

Rational Rational::fracNorm(Rational num) {
  if ((num.numer < 0) && (num.denom)){
    Rational newFrac (fabs(num.numer), fabs(num.denom));
    return newFrac;
  } else if (num.denom < 0) {
    Rational newFrac (-num.numer, fabs(num.denom));
    return newFrac;
  }
}
}