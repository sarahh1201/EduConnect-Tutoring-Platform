//main.cpp
#include <iostream>
#include "Request.h"
using namespace std;


int main() {
    vector<Tutor> tutors = {
		Tutor(1, "Alice", {"math_101", "physics_099"}, 4.8, true),//id,name, subject(s), rating, availability
        Tutor(2, "Bob", {"history_100"}, 4.2, true),
        Tutor(3, "Carol", {"math_102","math_103"}, 3.9, false),
		Tutor(4, "David", {"math_101", "chemistry_101"}, 4.5, true)
    };

    Request req(101, "math_101", "High", "note");//could use button for front end
    vector<Tutor> matches = req.matchTutors(tutors);

	int count = 1;
	cout << "Matched Tutors for Request for " <<req.subject << " :" << endl;
	for (const auto& t : matches) {//could change availability to set times
       cout << count<<") "<< "Tutor: " << t.name<< " | Rating: " << t.rating<< " | Available: " << (t.available ? "Yes" : "No")<<endl;
       count++;
    }

    return 0;
}
