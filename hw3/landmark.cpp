#include <iostream>
#include <string>
using namespace std;

class Landmark {
  public:
    Landmark(string name) : m_name(name) {};
    virtual ~Landmark() {};
    virtual string color() const = 0;
    virtual string icon() const = 0;
    string name() const {
      return m_name;
    }
  private:
    string m_name;

};

class Hotel : public Landmark {
  public:
    Hotel(string name)
    : Landmark(name)
    {};

    virtual ~Hotel() {
      cout << "Destroying the hotel " << name() << "." << endl;
    }

    virtual string color() const {
      return "yellow";
    }

    virtual string icon() const {
      return "bed";
    }
};

class Restaurant : public Landmark {
  public:
    Restaurant(string name, int cap)
    : Landmark(name), m_cap(cap)
    {};

    virtual ~Restaurant() {
      cout << "Destroying the restaurant " << name() << "." << endl;
    }

    virtual string color() const {
      return "yellow";
    }

    virtual string icon() const {
      return m_cap >= 40 ? "large knife/fork" : "small knife/fork";
    }
  private:
    int m_cap;
};

class Hospital : public Landmark {
  public:
    Hospital(string name)
    : Landmark(name)
    {};

    virtual ~Hospital() {
      cout << "Destroying the hospital " << name() << "." << endl;
    }

    virtual string color() const {
      return "blue";
    }

    virtual string icon() const {
      return "H";
    }
};

void display(const Landmark* lm)
{
    cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
         << lm->name() << "." << endl;
}

int main()
{
    Landmark* landmarks[4];
    landmarks[0] = new Hotel("Westwood Rest Good");
      // Restaurants have a name and seating capacity.  Restaurants with a
      // capacity under 40 have a small knife/fork icon; those with a capacity
      // 40 or over have a large knife/fork icon.
    landmarks[1] = new Restaurant("Bruin Bite", 30);
    landmarks[2] = new Restaurant("La Morsure de l'Ours", 100);
    landmarks[3] = new Hospital("UCLA Medical Center");

    cout << "Here are the landmarks." << endl;
    for (int k = 0; k < 4; k++)
        display(landmarks[k]);

      // Clean up the landmarks before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete landmarks[k];
}
