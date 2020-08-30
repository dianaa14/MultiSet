

#include <iostream>
#include "MultiSet.h"
#include"Comparator.h"
#include "Node.h"
#include<cassert>


using namespace std;
int main()
{
//verificari constructor
    //int 
    MultiSet<int> a;
    a.insert(10);
    a.insert(70);
    a.insert(60);
    a.insert(1);
    a.insert(70);
    a.insert(70);

    assert(a.getSize() == 6);

    //char
    MultiSet<char> b;
    b.insert('a');
    b.insert('b');
    b.insert('c');


    assert(b.getSize() == 3);

    //string
    MultiSet<string> c;
    c.insert("portofel");
    c.insert("pix");
    c.insert("ochelari");
    c.insert("portofel");
    c.insert("portofel");


    assert(c.getSize() == 5);
   

    //double
    MultiSet<double> d;
    d.insert(2.22);// valori care au 
    d.insert(3.22);//aceeasi parte zecimala deci vor fi considerate egale
    d.insert(6.23);
    d.insert(2.66);
 
    
    
    assert(a.getNr(70) == 3);

   
    
   //vrerificare daca face parte din MultiSet
    assert(c.IsInMultiset("pix") == true);

   //verificare nr aparitii element 
    assert(c.getNr("portofel") == 3);

    // verificare pentru stergerea primei aparitii a unui element
    a.deleteAparition(70);
    assert(a.getNr(70) == 2);

    //verificare stergerea nodului- stergerea tuturor aparitiilor
    c.deleteNode("portofel");
    assert(c.IsInMultiset("portofel") == false);// pentru ca am sters toate aparitiile elementului
    assert(c.getSize() == 2);//size- nr aparitii element sters

  
    assert(d.getNr(2.22) == 2);// 3.22 este considerat egal
 
   
  
    //verific destructorul
    c.~MultiSet();
    assert(c.getSize() == 0);

    
    
    cout << "cool";
    
}
