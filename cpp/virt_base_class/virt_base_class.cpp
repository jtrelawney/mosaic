#include <iostream>

class data{
public:
    data(){ std::cout << "data constructor" <<std::endl;}
    ~data(){ std::cout << "data de - structor" <<std::endl;}
    virtual void print(){std::cout << "data here" <<std::endl;}
    // this needs to be pure virtual
    // so that the derived objects are required to implement the function
    // if not declared virtual only then an implementation is required
    // a base class object cannot be created whene pure virtual function is declared, because it is not clear what funciton to call
    virtual int get_data_ptr()=0;
    //virtual int get_data_ptr(){std::cout << "call to data::get_data_ptr, do you want that?" <<std::endl;};
};

class camera_data:public data{
private:
    int data_ptr;
public:
    camera_data():data_ptr(1){ std::cout << "camera_data constructor" <<std::endl;}
    ~camera_data(){ std::cout << "camera_data de - structor" <<std::endl;}
    void print(){std::cout << "camera_data here" <<std::endl;}
    int get_data_ptr(){ return data_ptr; }
};

class other_data:public data{
private:
    int data_whatever;
public:
    other_data():data_whatever(2){ std::cout << "other_data constructor" <<std::endl;}
    ~other_data(){ std::cout << "other_data de - structor" <<std::endl;}
    void print(){std::cout << "other_data here" <<std::endl;}
    int get_data_ptr(){ return data_whatever; }
};

int main(){
    /*
    std::cout << "\n\ncreate data base object" << std::endl;
    std::cout << "this will not compile in case of a pure virtual function declaration in base" << std::endl;
    data dt0;
    dt0.print();
    std::cout << dt0.get_data_ptr() << std::endl;
    */
    camera_data ct;
    data &dt1{ct};
    std::cout << "\n\ncreate camera data and its base" << std::endl;
    ct.print();
    std::cout << ct.get_data_ptr() << std::endl;
    dt1.print();
    std::cout << dt1.get_data_ptr() << std::endl;

    std::cout << "\n\ncreate other data using a ptr to its base" << std::endl;
    data *dt2 = new other_data();
    dt2->print();
    std::cout << dt2->get_data_ptr() << std::endl;
}
