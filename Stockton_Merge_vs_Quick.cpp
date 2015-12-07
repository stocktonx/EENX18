//
//  EEN318
//
//  MergeSort vs. QuickSort
//
//  Created by Andrew Stockton on 11/17/14.
//  Copyright (c) 2014 Andrew Stockton. All rights reserved.
//

#include <library.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;

const bool debugging = false;

double getcputime(void) {
    struct timeval tim;
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    tim=ru.ru_utime;
    double t=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
    tim=ru.ru_stime;
    t+=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
    return t; }

struct person {
    string fname, lname, state;
    int social, dob, zip;
    person(){
        social = NULL;
        dob = NULL;
        zip = NULL;
        fname = "";
        lname = "";
        state = "";
    }
    
    person(int d, int SSN, string f, string l, string s, int z)
    {   social = SSN;
        fname = f;
        lname = l;
        dob = d;
        state = s;
        zip = z; }
    
    void print()
    { cout << dob << " " << fname << " " << lname << " " << endl; }
    
    bool lessthan(person * c) {
        int lcomp = lname.compare(c->lname);
        if(lcomp < 0 ) {
            return true;
        }
        else if(lcomp > 0) {
            return false;
        }
        else {
            int fcomp = fname.compare(c->fname);
            if(fcomp < 0 ) {
                return true;
            }
            else if(fcomp > 0) {
                return false;
            }
            else {
                if(dob < c->dob) {
                    return true;
                }
                else
                    return false;
            }
        }
    }
};

person * * read_file(person * * rfile, int f_size, string fileinput) {
    string filen;
    if(f_size < 0 || f_size > 100000) {
        cout << "not a valid number : ( \n";
        exit(1); }
    else if(f_size <= 1000 && fileinput == "data1.txt") {
        filen = "/home/www/class/een318/database1.txt"; }
    else if(f_size <= 2000 && fileinput == "data2.txt") {
        filen = "/home/www/class/een318/database2.txt"; }
    else if(f_size <= 3000 && fileinput == "data3.txt") {
        filen = "/home/www/class/een318/database3.txt"; }
    else if(f_size <= 5000 && fileinput == "data5.txt") {
        filen = "/home/www/class/een318/database5.txt"; }
    else if(f_size <= 10000 && fileinput == "data10.txt") {
        filen = "/home/www/class/een318/database10.txt"; }
    else if(f_size <= 20000 && fileinput == "data20.txt") {
        filen = "/home/www/class/een318/database20.txt"; }
    else if(f_size <= 30000 && fileinput == "data30.txt") {
        filen = "/home/www/class/een318/database30.txt"; }
    else if(f_size <= 50000 && fileinput == "data50.txt") {
        filen = "/home/www/class/een318/database50.txt"; }
    else if(f_size <= 100000 && fileinput == "data100.txt") {
        filen = "/home/www/class/een318/database100.txt"; }
    ifstream fin(filen.c_str());
    if (fin.fail())
    { cout << "Failed to open file\n"; exit(1); }
    int i = 0;
    while (!fin.fail() && i<f_size)
    { string first, last, state;
        int soc, dob, zipc;
        if ( fin.fail() ) {  break; }
        fin >> dob >> soc >> first >> last >> state >> zipc;
        person * p = new person(dob, soc, first, last, state, zipc);
        rfile[i] = p;
        if(debugging) cout << i << endl;
        i++;
    }
    fin.close();
    return rfile;
}

void merge(person * * array, const int & beg, const int & end, const int & mid)
{
    int b = beg;
    int m = mid;
    int c = 0;
    int length = (end - beg)+1;
    
    person * * temp_array = new person * [length];  // Create temporary array to hold in-order objects

    while(b < mid && m <= end) {    // Do comparisons on both, fill new array, until one of the 2 sets are empty
        if(debugging) { cout << "   b is " << b << " and m is " << m << " and c is " << c << endl; }
        if(array[b]->lessthan(array[m])) {
                temp_array[c] = array[b];
                b++; }
        else { 
            temp_array[c] = array[m];
            m++;
        }
        c++;
    }
    
    if(b == mid) {      // find which set is empty, copy over everything else!
        while(m <= end) {
            temp_array[c] = array[m];
            m++;
            c++;
        }
    }
    else{
        while(b < mid) {
            temp_array[c] = array[b];
            b++;
            c++;
        }
    }
                        // Now that we've filled the new in-order array, lets copy it to our original
    for(int x = 0; x<length; x++) {
        array[beg + x] = temp_array[x]; }
    
    delete[] temp_array;    // delete the old in-order array to save memory
}

void mergesort(person * * file, const int & a, const int & z) // a = beginning, z = ending
{   if (a < z)
    {
        int middle=(a+z)/2;
        mergesort(file,a,middle);
        mergesort(file,middle+1,z);
        merge(file,a,z,middle+1);
    }
}

void swap_people(person * &A, person * &B) {
    person * temp = A;
    A = B;
    B = temp;
}

int partition(person * database[], const int & beg, const int & end) {
    
        int pivot = rand() % (beg-end)+1 + beg; 
	int left = beg, right = end;

	while (left <= right) {
		while (left <= end) {
            if ( ! database[left]->lessthan(database[pivot])) break;
			left++;							
		}
		while (right >= beg) {
            if (database[right]->lessthan(database[pivot])) break;
			right--;						
		}
		if (left <= right) {
			swap(database[left],database[right]);
			left++;
			right--;
		}
	}
	if (left < pivot && database[pivot]->lessthan(database[left])) {
		swap(database[left],database[pivot]);
        pivot = left;
	}
    else if (right > pivot && database[right]->lessthan(database[pivot])) {
		swap(database[right],database[pivot]);
        pivot = right;
	}
	return pivot;
}


void quicksort(person * * database, const int & beg, const int & end) {
   if(end > beg) {
        int pivot = partition(database,beg,end);
        quicksort(database, beg, pivot-1);
        quicksort(database,pivot+1, end);
   }
}

void outputfile(person * * thefile, int thesize) {
    ofstream fout("sorted.txt");
    for(int x = 0; x<thesize; x++) { 
    	fout << thefile[x]->dob << " " << thefile[x]->social << " "
	     << thefile[x]->fname << " " << thefile[x]->lname << " "
	     << thefile[x]->state << " " << thefile[x]->zip << endl;
    }
};

int main( int argc, char *argv[] ) {    
    int fsize = atoi(argv[2]);
    person * * theBase = new person* [fsize];
    theBase = read_file(theBase, fsize, argv[3]);
    
    // Output unsorted array of persons
    if(debugging) {
        for(int a = 0; a<fsize; a++){
            cout << a<< " ";
            theBase[a]->print();
        }
    }
    
    double mtime = getcputime();
    string sort = argv[1];
    if(sort == "Q" || sort == "q") quicksort(theBase, 0, fsize-1);    
    else if(sort == "M" || sort == "m") mergesort(theBase, 0, fsize-1);
    double mtimeend = getcputime();
    double totaltime = mtimeend - mtime;

    // Output sorted array of persons
    if(debugging) {
        for(int a = 0; a<fsize; a++){
            cout << a<< " ";
            if(theBase[a] != NULL) {cout << "    "; theBase[a]->print();}
        }
    }
    
    if(sort == "q")
    	cout << "\nQUICKSORT: this took " << totaltime << " seconds to run\n\n";
    if(sort == "m")
	cout << "\nMERGESORT: this took " << totaltime << " seconds to run\n\n";
	
    outputfile(theBase, fsize);
    return 0;
}
