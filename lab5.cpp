#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "extrafile.h"
using namespace std;

//structure to define a weapon with name, rate of power, and consumption of power
struct Weapon {
    Weapon(string nameofWeapon, int rateofPower, float consumpOfPower);
    int rateofPower;
    float consumpOfPower;
    string nameofWeapon;
};
//Weapon constructor definition 
Weapon::Weapon(string nameofWeapon, int rateofPower, float consumpOfPower) {
    this->nameofWeapon = nameofWeapon;
    this->rateofPower = rateofPower;
    this->consumpOfPower = consumpOfPower;
}
//class to represent starship with name, class, length, capacity, warp speed, and number of weapons
class Starship {
    string nameofShip;
    string classofShip;
    int lengthofShip;
    int shieldCap;
    float warpSpeed;
    unsigned int numWeapons;
public:
    vector<Weapon> weapons;
    vector<Starship> starships;
    Starship(string nameofShip, string classofShip, int lengthofShip, int shieldCap, float warpSpeed, int numWeapons);
    void loadData(string nameofFile);
    void print();
    int GetPower();
    string GetName() { 
        return nameofShip; 
    }
    string GetClass() { 
        return classofShip; 
    }
    int GetshieldCap() { 
        return shieldCap; 
    }
    float GetWarpSpeed() { 
        return warpSpeed; 
    }
    int GetNumWeapons() { 
        return numWeapons; 
    }
    int GetWeaponPower(int i) { 
        return weapons.at(i).rateofPower; 
    }
};
//starship constructor definition 
Starship::Starship(string nameofShip, string classofShip, int lengthofShip, int shieldCap, float warpSpeed, int numWeapons) {
    this->nameofShip = nameofShip;
    this->classofShip = classofShip;
    this->lengthofShip = lengthofShip;
    this->shieldCap = shieldCap;
    this->warpSpeed = warpSpeed;
    this->numWeapons = numWeapons;
}

//function that calculates total power of all weapons on a ship
int Starship::GetPower() {
    int totalPower = 0;
    //loop through each weapon and calculate the total of their power
    for (unsigned int i = 0; i < numWeapons; i++)
        totalPower = totalPower + (weapons.at(i).rateofPower);
    return totalPower;
}
//load ship data from binary file
vector<Starship> loadData(string nameofFile) {
    vector<Starship> tempStorage; //temporary storage for ships
    unsigned int countofShip = 0; //number of ships read from file
    unsigned int lengthofShip = 0;
    unsigned int lengthofClass = 0;
    unsigned int shieldCap = 0;
    unsigned int numWeapons = 0;
    unsigned int weaponNameLength = 0;
    int rateofPower = 0;
    float warpSpeed = 0.0;
    float consumpOfPower = 0.0;
    string nameofShip = "";
    string nameofWeapon = "";
    string classofShip = "";
    //open file in binary mode
    ifstream inFile(nameofFile, ios_base::binary);
    if (inFile.is_open()) {
        //read number of ships from that file
        inFile.read((char*)&countofShip, 4);
        for (unsigned int i = 0; i < countofShip; i++) {
            //read length of ship's name and actual name
            inFile.read((char*)&lengthofShip, 4);
            char* temp = new char[lengthofShip + 1];
            inFile.read(temp, lengthofShip * sizeof(temp[0]));
            nameofShip = temp;
            delete[] temp;

            //read the length of the ship's class and actual class
            inFile.read((char*)&lengthofClass, 4);
            temp = new char[lengthofClass + 1];
            inFile.read(temp, lengthofClass * sizeof(temp[0]));
            classofShip = temp;
            delete[] temp;
            //read the ship's length, capacity, warp speed, and number of weapons 
            inFile.read((char*)&lengthofShip, 2);
            inFile.read((char*)&shieldCap, 4);
            inFile.read((char*)&warpSpeed, 4);
            inFile.read((char*)&numWeapons, 4);
            //create a new ship object with the read data 
            Starship newObj = Starship(nameofShip, classofShip, lengthofShip, shieldCap, warpSpeed, numWeapons);
            //if the ship has weapons, read weapon details 
            if (numWeapons != 0) {
                for (unsigned int i = 0; i < numWeapons; i++) {
                    inFile.read((char*)&weaponNameLength, 4);
                    temp = new char[weaponNameLength + 1];
                    inFile.read(temp, weaponNameLength * sizeof(temp[0]));
                    nameofWeapon = temp;
                    delete[] temp;              
                    inFile.read((char*)&rateofPower, 4);         
                    inFile.read((char*)&consumpOfPower, 4);
                    newObj.weapons.push_back(Weapon(nameofWeapon, rateofPower, consumpOfPower));
                }
            }
            tempStorage.push_back(newObj);
        }
        inFile.close();
    }
    return tempStorage;
}
//function to print out details of the ship
void Starship::print() {
    cout << "Name: " << nameofShip << endl;
    cout << "Class: " << classofShip << endl;
    cout << "Length: " << lengthofShip << endl;
    cout << "Shield capacity: " << shieldCap << endl;
    cout << "Maximum Warp: " << warpSpeed << endl;
    cout << "Armaments: " << endl;
    //check whether or not the ship has weapons, if so, print out their details
    if (numWeapons != 0) {
        for (unsigned int i = 0; i < numWeapons; i++) {
            cout << weapons.at(i).nameofWeapon << ",";
            cout << " " << weapons.at(i).rateofPower << ",";
            cout << " " << weapons.at(i).consumpOfPower << endl;
        }
        cout << "Total firepower: " << GetPower() << endl;
    }
    else {
        cout << "Unarmed" << endl;
    }
    cout << endl;
}

int main() {
    vector<Starship> starships; 
    cout << "Which file(s) to open?\n";
    cout << "1. friendlyships.shp" << endl;
    cout << "2. enemyships.shp" << endl;
    cout << "3. Both files" << endl;
    int option;
    cin >> option;

    //load files here
    switch (option) {
    case 1:
        starships = loadData("friendlyships.shp");
        break;
    case 2:
        starships = loadData("enemyships.shp");
        break;
    case 3:
        starships = loadData("friendlyships.shp");
        vector<Starship> appendShip = loadData("enemyships.shp");
        starships.insert(starships.end(), appendShip.begin(), appendShip.end());
        break;
    }

    cout << "1. Print all ships" << endl;
    cout << "2. Starship with the strongest weapon" << endl;
    cout << "3. Strongest starship overall" << endl;
    cout << "4. Weakest ship (ignoring unarmed)" << endl;
    cout << "5. Unarmed ships" << endl;

    cin >> option;

    //work your magic here
    switch (option) {

    case 1: {
        //print details of all starships
        for (unsigned int i = 0; i < starships.size(); i++)
            starships.at(i).print();
        break;
    }
    //find ship with strongest weapon 
    case 2: {
        int newPower;
        int index = 0;
        int mostPowerWeapon = 0;
        for (unsigned int i = 0; i < starships.size(); i++) {

            if (starships.at(i).GetNumWeapons() > 0) {
                for (int k = 0; k < starships.at(i).GetNumWeapons(); k++) {
                    newPower = starships.at(i).GetWeaponPower(k);
                    if (newPower > mostPowerWeapon) {
                        mostPowerWeapon = newPower;
                        index = i;
                    }
                }
            }
        }
        starships.at(index).print();
        break;
    }
    //find ship with strongest firepower 
    case 3: {
        int index = 0;
        int mostPowerShip = 0;
        int newPower = 0;
        for (unsigned int i = 0; i < starships.size(); i++) {

            if (starships.at(i).GetNumWeapons() > 0) {
                newPower = starships.at(i).GetPower();
                if (newPower > mostPowerShip) {
                    mostPowerShip = newPower;
                    index = i;
                }
            }
        }
        starships.at(index).print();
        break;
    }
    //find the ship with weakest firepower 
    case 4: {
        int index = 0;
        int leastPowerShip = 100000; 
        int newPower = 0;
        for (unsigned int i = 0; i < starships.size(); i++) {
            if (starships.at(i).GetNumWeapons() > 0) {
                newPower = starships.at(i).GetPower();
                if (newPower < leastPowerShip) {
                    leastPowerShip = newPower;
                    index = i;
                }
            }
        }
        starships.at(index).print();
        break;
    }
    //find the all unarmed ships 
    case 5:
        for (unsigned int i = 0; i < starships.size(); i++) {
            if (starships.at(i).GetNumWeapons() == 0) {
                starships.at(i).print();
            }
        }
        break;
    }
    return 0;
}