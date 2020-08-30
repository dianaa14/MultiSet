#pragma once
#include<iostream>
#include"Node.h"
#include"Comparator.h"
#include<list>

using namespace std;


#pragma once
#include "Comparator.h"
#include "Node.h"
#include <list>
using namespace std;

template <typename T, typename F = Comparator<T>>
class MultiSet {
    Node <T>* root;
    int size;//numarul de elemente din MultiSet(inclusiv cele care se repeta)- marimea MultiSetului
    int copies;// numarul de elemente din MultiSet care apar de mai multe ori
    F compare;


public:
    MultiSet();
    MultiSet(const MultiSet<T, F>&);
    ~MultiSet();

    
    void insert(const T&);
    void deleteAparition(const T&);//sterge prima aparitie 
    void deleteNode(const T&);//sterge nodul cu toate aparitiile
    

    int getSize() const; 
    int getDistinctSize()const;
    int getNr(T);

    bool IsInMultiset(const T&) const;
   

    template <typename U, typename F>
    friend ostream& operator <<(ostream&, const MultiSet<U, F>&);
    
    MultiSet<T, F>& operator =(const MultiSet<T, F>&);

protected:
    void destructorHelper(Node<T>*);

    //Metode folosite pentru insert
    void fixInsertion(Node<T>*);
    void rotateLeft(Node<T>*);
    void rotateRight(Node<T>*);

    //Metode folosite pentru deleteAparitions si deleteNode
    void removeNode(Node<T>*);
    Node<T>* min(Node<T>*) const;
    Node<T>* search(const T&);
    void fixRemoval(Node<T>*);


};


//Constructor fara parametrii
template<typename T, typename F>
MultiSet<T, F>::MultiSet() :size(0), copies(0), root(NULL){}


//Constructor de copiere
template<typename T, typename F>
MultiSet<T, F>::MultiSet(const MultiSet<T, F>& s) {
    if (s.size == 0)
        return;
    list <Node<T>*> queue;
    queue.push_back(s.root);
    while (queue.size()) {
        this->insert(queue.front()->info);
        if (queue.front()->left)
            queue.push_back(queue.front()->left);
        if (queue.front()->right)
            queue.push_back(queue.front()->right);
        queue.pop_front();
    }
}



//Functie recursiva pentru a sterge elementele 
template <typename T, typename F>
void MultiSet<T, F>::destructorHelper(Node<T>* n) {
    if (n!=NULL) {
        destructorHelper(n->left);
        destructorHelper(n->right);
        delete n;
    }
}


//Destructor
template<typename T, typename F>
MultiSet<T, F>::~MultiSet() {
    this->destructorHelper(root);
    root = NULL;
    size = 0;
    copies = 0;
}


//Supraincarcare operator de atribuire "="
template<typename T, typename F>
MultiSet<T, F>& MultiSet<T, F>::operator=(const MultiSet<T, F>& s) {
    if (&s == this) {
        return *this;
    }

    this->~MultiSet();//golesc multiSetul

    if (s.size == 0) {
        return *this;
    }

    list <Node<T>*> copy;
    copy.push_back(s.root);
    while (copy.size()) {
        this->insert(copy.front()->info);
        if (copy.front()->left)
            copy.push_back(copy.front()->left);
        if (copy.front()->right)
            copy.push_back(copy.front()->right);
        copy.pop_front();
    }
    return *this;
}


//Roratie spre stanga
template<typename T, typename F>
void MultiSet<T, F>::rotateLeft(Node<T>* n) {
    //n face rotatie cu fiul drept, prin urmare
    //parintele nodului n va deveni parintele fiului drept
    //fiul drept va deveni parinte pentru nodul n 
    //iar fiul stang al fiului drept al lui n (adica nepotul lui n)
    //devine fiul drept al lui n 


    Node<T>* nRight = n->right; //fiul drept
    nRight->parent = n->parent;
    if (n->parent == NULL)
        root = nRight;
    else {
        if (n == n->parent->left)
            n->parent->left = nRight;
        else
            n->parent->right = nRight;
    }
    n->parent = nRight;
    n->right = nRight->left;
    if (n->right != NULL)
        n->right->parent = n;
    nRight->left = n;
}




//Rotatie spre dreapta
template<typename T, typename F>
void MultiSet<T, F>::rotateRight(Node<T>* n) {
   // n face rotatie cu fiul stang, prin urmare
   //parintele nodului n va deveni parintele fiului stang
   //fiul stang va deveni parinte pentru nodul n 
   //iar fiul drept al fiului stang al lui n (adica nepotul lui n)
   //devine fiul stang al lui n 
   


    Node<T>* nLeft = n->left;
    nLeft->parent = n->parent;
    if (nLeft->parent == NULL)
        root = nLeft;
    else {
        if (n == n->parent->left)
            n->parent->left = nLeft;
        else
            n->parent->right = nLeft;
    }
    n->parent = nLeft;
    n->left = nLeft->right;
    if (n->left != NULL)
        n->left->parent = n;
    nLeft->right = n;
}






//Metoda pentru inserare element in MultiSet
template<typename T, typename F>
void MultiSet<T, F>::insert(const T& t) {
    //Daca valoarea se mai gaseste in MultiSet, ok va deveni 0 si nodul respectiv va avea nr++
    int ok = 1;
    this->size++;// MultiSet-ul va creste indiferent daca elementul a mai fost inserat
    Node<T>* newNode = new Node<T>(t);  //Nodul de inserat
    Node<T>* previous = NULL;           //Nodul anterior, cel care va deveni parinte
    Node<T>* n = root;

    //Daca multisetul e gol
    if (n == NULL) {
        root = newNode;
        root->increaseNr();

    }
    else {
        newNode->increaseNr();

        while (n != NULL && ok == 1)
        {
            previous = n;
            if (compare.equal(t, n->info)) // Daca mai apare in MultiSet nr++
            {
                n->increaseNr();
                ok = 0;
                copies++;
            }
            else {

                if (compare.less(t, n->info)) // t < n->info
                    n = n->left;
                else {
                    if (!compare.less(t, n->info) && compare.less(n->info, t))
                        n = n->right;
                }
            }

        }

        if (ok) {
            if (compare.less(t, previous->info))
                previous->left = newNode;
            else
                previous->right = newNode;
            newNode->parent = previous;
        }
        else {
            newNode->increaseNr();

        }

    }

    //Dupa ce inserez nodul, fac modificari pentru a respecta conditiile Red Black Tree
    if (ok)
        fixInsertion(newNode);
}




//Functie pentru pastrarea proprietatilor Red Black Tree dupa inserare 
template<typename T, typename F>
void MultiSet<T, F>::fixInsertion(Node<T>* newNode) {
    //Daca nodul si parintele sunt rosii, se incalca o regula Red Black Tree
    while (newNode != root && newNode->color == RED && newNode->parent->color == RED) {
        Node<T>* parentNode = newNode->parent;
        Node<T>* grandParentNode = parentNode->parent;
        Node<T>* uncleNode = NULL;

        //STANGA
        if (parentNode == grandParentNode->left) {
            uncleNode = grandParentNode->right;
            //Daca unchiul e NULL sau are culoarea rosie, atunci schimba culoarea
            //parinte =black, uncchi =black, bunic=red
            //Verific daca prin schimbarea culorii, bunicul incalca vreo regula
            if (uncleNode != NULL && uncleNode->color == RED) {
                grandParentNode->color = RED;
                parentNode->color = BLACK;
                uncleNode->color = BLACK;
                newNode = grandParentNode;
            }
            else {
                //Daca unchiul e NULL sau are culoarea neagra 

                //STANGA-DREAPTA :fac rotatie stanga(parinte),
                //iar apoi rotatie dreapta(bunic)
                if (newNode == parentNode->right) {
                    newNode = parentNode;
                    rotateLeft(newNode);
                }

                //STANGA-STANGA: rotatie dreapta(bunic)
                //Parent primeste culoarea neagra,
                //iar fostul bunic culoarea rosie
                rotateRight(grandParentNode);
                newNode->parent->color = BLACK;
                grandParentNode->color = RED;
            }
        }
        //DREAPTA ( invers fata de partea STANGA)
        else {
            uncleNode = grandParentNode->left;
            //Daca unchiul e NULL sau are culoarea rosie, atunci schimb culorile
            //parinte=black, unchie=black, bunic=red
            //Verific daca prin schimbarea culorii, bunicul incalca vreo regula
            if (uncleNode != NULL && uncleNode->color == RED) {
                grandParentNode->color = RED;
                parentNode->color = BLACK;
                uncleNode->color = BLACK;
                newNode = grandParentNode;
            }
            else {
                //Daca unchiul e NULL sau are culoarea neagra 
                //DREAPTA STANGA: rotatie dreapta(parinte),
                //iar apoi rotatie stanga(bunic)
                if (newNode == parentNode->left) {
                    newNode = parentNode;
                    rotateRight(newNode);
                }

                //DREAPTA DREAPTA: rotatie stanga(bunic)
                //Parintele primeste culoarea neagra,
                //iar fostul bunic culoarea rosie
                rotateLeft(grandParentNode);
                newNode->parent->color = BLACK;
                grandParentNode->color = RED;
            }
        }
    }
    //Radacina e mereu black
    root->color = BLACK;
}

//Metoda pentru stergere element din MultiSet
template<typename T, typename F>
void MultiSet<T, F>::removeNode(Node<T>* del) {

    Node<T>* replace = NULL;
    if (del->right == NULL && del->left == NULL)
        replace = NULL;
    if (del->right == NULL && del->left != NULL)
        replace = del->left;
    if (del->right != NULL && del->left == NULL)
        replace = del->right;
    if (del->right != NULL && del->left != NULL)
        replace = min(del->right);


    //Daca del e frunza
    if (replace == NULL) {
        //Daca e radacina, inseamna ca multiset-ul devine gol
        if (del == root)
            root = NULL;
        else {
            //Daca del are culoarea neagra -> double black
            //Trebuie sa verific daca se pastreaza proprietatile Red Black Tree
            if (del->color == BLACK)
                fixRemoval(del);
            //Daca este rosu, doar sterg pentru ca nu influenteaza proprietatile
            //Sau dupa ce se rezolva fixRemoval, in cazul in care este negru
            if (del == del->parent->left)
                del->parent->left = NULL;
            else
                del->parent->right = NULL;
            delete del;
        }
    }
    else {
        //Daca del are 2 copii
        if (del->left != NULL && del->right != NULL) {
            //Interschimb valoarea celor doua noduri(replace si nodeDelete)
            //Apelez removeNode pentru replace (care va avea maxim un copil)
            T val = del->info;
            del->info = replace->info;
            replace->info = val;
            removeNode(replace);
        }
        //Daca del are un copil
        else {
            //Daca nodul de sters este radacina
            //Replace devine noua radacina (va fi negru obligatoriu)
            if (del == root) {
                replace->parent = NULL;
                replace->color = BLACK;
                root = replace;
                delete del;
            }
            else {
                //Refac legaturile
                if (del == del->parent->left)
                    del->parent->left = replace;
                else
                    del->parent->right = replace;
                if (replace != NULL)
                    replace->parent = del->parent;

                //Daca replace negru si del negru -> double black => trebuie sa verific prop
                if ((replace->color == BLACK || replace == NULL) && del->color == BLACK) {
                    del->info = replace->info;
                    if (replace == del->left) {
                        replace->right = del->right;
                        if (del->right != NULL)
                            del->right->parent = replace;
                    }
                    else {
                        replace->left = del->left;
                        if (del->left != NULL)
                            del->left->parent = replace;
                    }
                    delete replace;
                    fixRemoval(del);
                }
                else {
                    //Daca au culori diferite, atunci mut replace si il fac negru
                    replace->color = BLACK;
                    if (replace == del->left) {
                        replace->right = del->right;
                        if (del->right != NULL)
                            del->right->parent = replace;
                    }
                    else {
                        replace->left = del->left;
                        if (del->left != NULL)
                            del->left->parent = replace;
                    }
                    delete del;
                }
            }
        }
    }


}
//Functie pentru pastrarea proprietatilor Red Black Tree dupa stergere
template<typename T, typename F>
void MultiSet<T, F>::fixRemoval(Node<T>* n) {
    if (n == root)
        return;

    Node<T>* parent = n->parent;
    Node<T>* sibling;
    if (parent == NULL)
        sibling = NULL;
    if (n == n->parent->left)
        sibling = parent->right;
    else
        sibling = parent->left;

    if (sibling == NULL)
        fixRemoval(parent);
    else {
        //Daca sibling rosu : schimb culoarea(parent,sibling)
        //Rotate(parinte), transformandu-se in unul din cazurile cand sibling negru: fixRemoval(n)
        if (sibling->color == RED) {
            parent->color = RED;
            sibling->color = BLACK;
            //Daca sibling = left child of parent -> rightRotate(parent)
            if (sibling == sibling->parent->left)
                rotateRight(parent);
            //Daca sibling = right child of parent -> leftRotate(parent)
            else
                rotateLeft(parent);
            fixRemoval(n);
        }
        else {
            //Daca sibling negru si ambii copii negru(sau NULL):
            //Renuntam la "un negru" de la n(double black) si sibling(black)
            // => n are culoarea neagra si sibling rosie
            //Adaug o culoare neagra extra la parinte (parintele devine negru sau double-black)
            //Apelez fixRemoval(parinte) daca parintele este double-black
            if (sibling->color == BLACK && (sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                sibling->color = RED;
                if (parent->color == BLACK)
                    fixRemoval(parent);
                else
                    parent->color = BLACK;
            }
            //Daca sibling negru si are cel putin un fiu de culoare rosie
            else {
                //Daca sibling negru si fiul stang este rosu (deci, fiul drept are culoarea neagra):
                //sibling = red, leftChild = black (schimb culorile(leftChild,sibling))
                //rightRotate(sibling) => se transforma in alt caz
                if (sibling->left != NULL and sibling->left->color == RED) {
                    sibling->color = RED;
                    sibling->left->color = BLACK;
                    rotateRight(sibling);
                    fixRemoval(n);
                }
                //Daca sibling negru si fiul drept este rosu (deci, fiul stang are culoarea neagra):
                //rightChild = black si schimb culorile (sibling, parent)
                //leftRotate(parent)
                if (sibling->right != NULL and sibling->right->color == RED) {
                    sibling->right->color = BLACK;
                    sibling->color = parent->color;
                    parent->color = BLACK;
                    rotateLeft(parent);
                }
            }
        }
    }
}


//Metoda pentru stergerea unui element din MultiSet (prima aparitie)
template<typename T, typename F>
void MultiSet<T, F>::deleteAparition(const T& t) {


    Node<T>* current = search(t);
    if (current->nr > 1) {
        size--;
        current->decreaseNr();
    }
    else
        if (current != NULL) {
            size--;
            removeNode(current);
        }
}
//Metoda pentru stergerea elementului din MultiSet (sterge elementul cu toate aparitiile lui)
template<typename T, typename F>
void MultiSet<T, F>::deleteNode(const T& t) {


    Node<T>* current = search(t);
    if (current != NULL) {
        size = size - current->nr;
        removeNode(current);
        current->nr = 0;
    }
}


//Metoda care returneaza numarul de elemente din MultiSet
template<typename T, typename F>
int MultiSet<T, F>::getSize() const {
    return this->size;
}
//Metoda care returneaza numarul de elemente dinstincte din MultiSet

template<typename T, typename F>
int MultiSet<T, F>::getDistinctSize()const {
    int x;
    x = size - copies;
    return x;
}


//Returneaza cel mai din stanga nod din subarborele unui nod
template<typename T, typename F>
Node<T>* MultiSet<T, F>::min(Node<T>* n) const {
    Node<T>* minNode = n;
    while (minNode->left != NULL)
        minNode = minNode->left;
    return minNode;
}

//Metoda care returneaza numarul de aparitii ale unui element


template<typename T, typename F>
int MultiSet<T, F>::getNr(T val) {
    Node<T>* pt;
    pt = search(val);
    return pt->nr;
}


//Metoda de cautare care returneaza nodul cu valoarea data
template<typename T, typename F>
Node<T>* MultiSet<T, F>::search(const T& t) {
    Node<T>* n = this->root;
    while (n) {
        if (!compare.less(n->info, t) && !compare.less(t, n->info))
            return n;
        else {
            if (!compare.less(n->info, t) && compare.less(t, n->info))
                n = n->left;
            else
                n = n->right;
        }
    }
    return NULL;
}




//Metoda care verifica daca un element se afla in MultiSet
template<typename T, typename F>
bool MultiSet<T, F>::IsInMultiset(const T& t) const {
    Node<T>* pt = this->root;
    if (pt == NULL)
        return false;
    while (pt) {
        if (!compare.less(pt->info, t) && !compare.less(t, pt->info))
            return true;
        else {
            if (!compare.less(pt->info, t) && compare.less(t, pt->info))
                pt = pt->left;
            else
                pt = pt->right;
        }
    }
    return false;
}


//Supraincarcarea operatorului de afisare "<<"
template<typename T, typename F>
ostream& operator<<(ostream& out, const MultiSet<T, F>& s) {
    if (s.root) {
        list <Node<T>*> queue;
        queue.push_back(s.root);
        while (queue.size()) {
            out << queue.front()->getInfo() << " ";
            if (queue.front()->getLeft())
                queue.push_back(queue.front()->getLeft());
            if (queue.front()->getRight())
                queue.push_back(queue.front()->getRight());
            queue.pop_front();
        }
    }
    return out;
}