 #include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Person class given from the book
//-----------------------------Person Class------------------------------------
class Person {
public:
  Person();
  Person(string the_name);
  Person(const Person& the_object);
  string get_name() const;
  Person& operator =(const Person& rt_side);
  friend istream &operator>>(istream &in_stream, Person& person_object);
  friend ostream &operator<<(ostream &out_person, const Person& person_object);
private:
  string name;
};

Person::Person() {
  name = "you";
}

Person::Person(string the_name){
  this->name = the_name;
}

Person::Person(const Person& the_object) {
   the_object;
}

string Person::get_name() const {
  return name;
}

Person& Person::operator= (const Person& rt_side){
  this->name = rt_side.name;
  return *this;
}  

istream &operator>>(istream &in_stream, Person &person_object) {;
  cout << "Enter Name: ";
  in_stream >> person_object.name;
  return in_stream;
}

ostream &operator<<(ostream &out_stream, const Person &person_object) {
  out_stream << person_object.get_name() << endl;
  return out_stream;
}


//----------------------------------Vehicle----------------------------------------------
class Vehicle {
public:
  Vehicle();
  Vehicle(string manName, int cyl, Person owner);
  string get_manName();
  int get_cyl();
  Person get_owner();
  void change_owner(Person newOwner);

protected:
  string manName;
  int cyl;
  Person owner;
  Person newOwner;
};


//-----------------------------------Truck-----------------------------------------------
class Truck : public Vehicle {
public:
  Truck();
  Truck(string manName, int cyl, Person owner, double loadCapacity, int towCapacity);
private:
  double loadCapacity;
  int towCapacity;
};


//------------------------------------Car-----------------------------------------------
class Car : public Vehicle {
public:
  Car();
  Car(string manName, int cyl, Person owner, int gasMileage);
protected:
  int gasMileage;
};

//----------------------------------SportsCar-----------------------------------------
class SportsCar : public Car {
public:
  SportsCar();
  SportsCar(string manName, int cyl, Person owner, int quarterSpeed);
private:
  int  quarterSpeed;
};

//----------------------------Vehicle Base Class---------------------------------------- 
Vehicle::Vehicle() {
  manName = "Toyota";
  cyl = 6;
  Person Spencer;
  owner = Spencer;
}

Vehicle::Vehicle(string manName, int cyl, Person owner) {
  this->manName=manName;
  this->cyl=cyl;
  this->owner=owner;
}

string Vehicle::get_manName() {
  return manName;
}

Person Vehicle::get_owner() {
  return owner;
}

void Vehicle::change_owner(Person newOwner) {
  this->owner = newOwner;
}

//----------------------------Truck Derived Class---------------------------------------
Truck::Truck() : Vehicle(){
  loadCapacity = 10;
  towCapacity = 7500;
}

Truck::Truck(string manName, int cyl, Person owner, double loadCapacity, int towCapacity) :
  Vehicle(manName,cyl,owner) {
  this->loadCapacity = loadCapacity;
  this->towCapacity = towCapacity;
}

//-------------------------------Car Derived Class--------------------------------------
Car::Car() : Vehicle() {
  gasMileage = 35;
}

Car::Car(string manName, int cyl, Person owner, int gasMileage) : 
  Vehicle(manName,cyl,owner) {
  this->gasMileage = gasMileage;
}

//--------------------------------SportsCar Derived Class-------------------------------
SportsCar::SportsCar() : Car() {
  quarterSpeed = 8;
}

SportsCar::SportsCar(string manName, int cyl, Person owner, int quarterSpeed) : Car(manName, cyl, owner, gasMileage) {
  this->quarterSpeed = quarterSpeed;
}

//----------------------------------Print Method--------------------------------------
void printInformation(Vehicle v) {
  cout << "owner: " << v.get_owner() << endl;
  cout << "make: " << v.get_manName() << endl;
}




//--------------------------------- Main -----------------------------------------------
int main() {
  Person Bill;
  Vehicle v ("Ford", 4, Bill);
  printInformation(v);
  Truck t;
  printInformation(t);

  Person jon;
  
  cout << jon;

  cin >> jon;
  
  cout << jon;

  return 0;
}

