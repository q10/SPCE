/* 
 * File:   main.cpp
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 12:50 PM
 */

#include "common.h"

/*
class Animal {
    public:
        int value;
        virtual void eat() { 
            std::cout << "I eat like a generic Animal." << std::endl; 
        }
        virtual ~Animal() { 
            value = 5;
        }
};
 
class Wolf : public Animal {
    public:
        void eat() { 
            std::cout << "I eat like a wolf!" << std::endl;
            value += 10;
        }
        virtual ~Wolf() { 
        }
};
 
class Fish : public Animal {
    public:
        void eat() { 
            std::cout << "I eat like a fish!" << std::endl; 
        }
        virtual ~Fish() { 
        }
};
 
class GoldFish : public Fish {
    public:
        int other_value;
        GoldFish(){
            other_value = 198;
        }
        void eat() { 
            std::cout << "I eat like a goldfish!" << std::endl; 
        }
        virtual ~GoldFish() {
        }
        
};
 
class OtherAnimal : public Animal {
        virtual ~OtherAnimal() { 
        }
};
 
void ps(GoldFish * fish) {
    cout << fish->other_value;
}

int main() {
    std::vector<Animal*> animals;
    animals.push_back(new Animal());
    animals.push_back(new Wolf());
    animals.push_back(new Fish());
    animals.push_back(new GoldFish());
    animals.push_back(new OtherAnimal());
 
    for (std::vector<Animal*>::const_iterator it = animals.begin(); it != animals.end(); ++it) {
        (*it)->eat();
    }
 
    cout << animals.size() << endl;
    cout << animals[0]->value << endl;
    cout << animals[1]->value << endl;
    ps((GoldFish *)animals[3]);
    // cout << animals[3]->other_value << endl; // CANNOT CALL SUBCLASS STUFF IF USING A SUPERCLASS POINTER
    // cout << (GoldFish *)animals[3]->other_value << endl; // ERROR ALSO
    return 0;
}
 */


int main(int argc, char** argv) {
    run_tests(argc, argv);
    /*
    read_program_flags(argc, argv);
    initialize();    
    run_mc();
    print_system_vars();
    print_system_config();
    save_config_to_file();
     */
    return 0;
}
