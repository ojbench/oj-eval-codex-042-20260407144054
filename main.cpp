#include <iostream>
#include "solution.h"

int main() {
    // Basic local test mirroring provided samples
    {
        std::cout << "======== Constructor Test ========\n";
        std::vector<int> node_bounds{10,20,30,40,50,60,70,80,90,97};
        SpeedCircularLinkedList<int, 97> scl(node_bounds);
        scl.print();
    }
    {
        std::cout << "============ Size Test ============\n";
        std::vector<int> node_bounds{10,30,40,50,53};
        SpeedCircularLinkedList<int, 53> scl(node_bounds);
        std::cout << "size = " << scl.size() << '\n';
    }
    {
        std::cout << "============ Put Test ============\n";
        std::vector<int> node_bounds{10,30,50,70,90,110,130,150,170,197};
        SpeedCircularLinkedList<int, 197> scl(node_bounds);
        scl.put("19260817", 0);
        scl.put("What a pity!", 1);
        scl.put("ACM2025", 6);
        scl.print();
    }
    {
        std::cout << "=========== Get Test =============\n";
        std::vector<int> node_bounds{50,250,450,800,1000,1300,1350,1600,1850,2017};
        SpeedCircularLinkedList<std::string, 2017> scl(node_bounds);
        scl.put("Butterworth", "07:50");
        scl.put("Ipoh", "09:37");
        scl.put("KL Sentral", "12:15");
        scl.put("Seremban", "13:38");
        scl.put("Gemas", "14:42");
        scl.put("Segamat", "14:57");
        std::cout << scl.get("KL Sentral") << '\n';
        std::cout << scl.get("Gemas") << '\n';
        std::cout << scl.get("Butterworth") << '\n';
    }
    return 0;
}

