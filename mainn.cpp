#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

long long mergeComparisons = 0;
long long bubbleComparisons = 0;
long long swapsCount = 0;

class Node {
public:
    int id;
    string name;
    string dept;
    int sem;
    float cgpa;
    int credits;
    int year;
    Node* next;

    Node(int i, string n, string d, int s, float c, int cr, int y) {
        id = i; name = n; dept = d;
        sem = s; cgpa = c; credits = cr; year = y;
        next = nullptr;
    }
};

class StudentList {
private:
    Node* head;

    void swapData(Node* a, Node* b) {
        swap(a->id, b->id);
        swap(a->name, b->name);
        swap(a->dept, b->dept);
        swap(a->sem, b->sem);
        swap(a->cgpa, b->cgpa);
        swap(a->credits, b->credits);
        swap(a->year, b->year);
        swapsCount++;
    }

    Node* mergeCGPA(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;

        mergeComparisons++;
        if (a->cgpa > b->cgpa ||
           (a->cgpa == b->cgpa && a->credits > b->credits)) {
            a->next = mergeCGPA(a->next, b);
            return a;
        } else {
            b->next = mergeCGPA(a, b->next);
            return b;
        }
    }

    void split(Node* src, Node*& a, Node*& b) {
        Node* slow = src;
        Node* fast = src->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        a = src;
        b = slow->next;
        slow->next = nullptr;
    }

    void mergeSortCGPA(Node*& node) {
        if (!node || !node->next) return;
        Node *a, *b;
        split(node, a, b);
        mergeSortCGPA(a);
        mergeSortCGPA(b);
        node = mergeCGPA(a, b);
    }

public:
    StudentList() { head = nullptr; }

    ~StudentList() {
        while (head) {
            Node* t = head;
            head = head->next;
            delete t;
        }
    }

    void addStudent(int id, string name, string dept,
                    int sem, float cgpa, int credits, int year) {
        Node* n = new Node(id, name, dept, sem, cgpa, credits, year);
        if (!head) {
            head = n;
            return;
        }
        Node* t = head;
        while (t->next) t = t->next;
        t->next = n;
    }

    StudentList* copyList() {
        StudentList* c = new StudentList();
        for (Node* t = head; t; t = t->next)
            c->addStudent(t->id, t->name, t->dept, t->sem,
                          t->cgpa, t->credits, t->year);
        return c;
    }

    void bubbleSortByYear() {
        bool swapped;
        do {
            swapped = false;
            Node* t = head;
            while (t && t->next) {
                bubbleComparisons++;
                if (t->year > t->next->year ||
                   (t->year == t->next->year && t->id > t->next->id)) {
                    swapData(t, t->next);
                    swapped = true;
                }
                t = t->next;
            }
        } while (swapped);
    }

    void createCGPARanking() {
        ofstream out("ranked_by_cgpa.txt");
        mergeSortCGPA(head);

        out << "RANKED BY CGPA\n\n";
        int rank = 1;
        for (Node* t = head; t; t = t->next) {
            out << rank++ << " | " << t->id << " | "
                << t->name << " | " << t->dept
                << " | " << fixed << setprecision(2)
                << t->cgpa << "\n";
        }
    }

    void createEnrollmentReport() {
        ofstream out("sorted_by_enrollment.txt");
        bubbleSortByYear();

        int year = -1, count = 0;
        float sum = 0;

        for (Node* t = head; t; t = t->next) {
            if (t->year != year) {
                if (count)
                    out << "Average CGPA: "
                        << fixed << setprecision(2)
                        << sum / count << "\n\n";
                year = t->year;
                out << "Enrollment Year: " << year << "\n";
                sum = 0; count = 0;
            }
            out << t->id << " " << t->name
                << " " << t->cgpa << "\n";
            sum += t->cgpa;
            count++;
        }
        if (count)
            out << "Average CGPA: "
                << fixed << setprecision(2)
                << sum / count << "\n";
    }

    void createDepartmentAnalysis() {
        ofstream out("department_analysis.txt");

        for (Node* d = head; d; d = d->next) {
            bool printed = false;
            for (Node* p = head; p != d; p = p->next)
                if (p->dept == d->dept) printed = true;
            if (printed) continue;

            int cnt = 0;
            float sum = 0, hi = 0, lo = 4;
            int credits = 0;

            out << "\nDepartment: " << d->dept << "\n";

            for (Node* t = head; t; t = t->next) {
                if (t->dept == d->dept) {
                    out << t->id << " " << t->name
                        << " Sem " << t->sem
                        << " CGPA " << t->cgpa << "\n";
                    cnt++;
                    sum += t->cgpa;
                    credits += t->credits;
                    if (t->cgpa > hi) hi = t->cgpa;
                    if (t->cgpa < lo) lo = t->cgpa;
                }
            }

            out << "Total Students: " << cnt << "\n";
            out << "Average CGPA: " << sum / cnt << "\n";
            out << "Highest CGPA: " << hi << "\n";
            out << "Lowest CGPA: " << lo << "\n";
            out << "Average Credits: " << credits / cnt << "\n";
        }
    }

    void createPerformanceTiers() {
        ofstream out("performance_tiers.txt");

        int total = 0;
        for (Node* t = head; t; t = t->next) total++;

        const string tier[5] = {
            "Elite", "High Achievers",
            "Good Standing", "Satisfactory",
            "Needs Improvement"
        };

        for (int i = 0; i < 5; i++) {
            int cnt = 0;
            out << "\n" << tier[i] << "\n";
            for (Node* t = head; t; t = t->next) {
                float c = t->cgpa;
                bool ok =
                    (i == 0 && c >= 3.7) ||
                    (i == 1 && c >= 3.3 && c < 3.7) ||
                    (i == 2 && c >= 3.0 && c < 3.3) ||
                    (i == 3 && c >= 2.5 && c < 3.0) ||
                    (i == 4 && c < 2.5);

                if (ok) {
                    out << t->name << " (" << c << ")\n";
                    cnt++;
                }
            }
            out << "Count: " << cnt
                << " Percentage: "
                << (cnt * 100.0 / total) << "%\n";
        }
    }

    void searchByID(int key) {
        ofstream out("search_results.txt", ios::app);
        for (Node* t = head; t; t = t->next)
            if (t->id == key)
                out << t->id << " " << t->name << " "
                    << t->dept << " " << t->cgpa << "\n";
    }
};

int main() {
    ifstream in("students_data.txt");
    ofstream issues("data_issues.txt");
    ofstream dup("duplicates_report.txt");

    StudentList master;
    int n;
    in >> n;
    in.ignore();

    for (int i = 0; i < n; i++) {
        int id, sem, credits, year;
        float cgpa;
        string name, dept;

        in >> id;
        in.ignore();
        getline(in, name);
        in >> dept >> sem >> cgpa >> credits >> year;

        if (cgpa < 0 || cgpa > 4 || sem < 1 || sem > 8)
            issues << "Invalid data for ID " << id << "\n";

        for (int j = 0; j < i; j++)
            ifstream("students_data.txt");

        master.addStudent(id, name, dept, sem, cgpa, credits, year);
    }

    StudentList* a = master.copyList();
    StudentList* b = master.copyList();
    StudentList* c = master.copyList();
    StudentList* d = master.copyList();

    a->createCGPARanking();
    b->createEnrollmentReport();
    c->createDepartmentAnalysis();
    d->createPerformanceTiers();

    ofstream perf("algorithm_performance.txt");
    perf << "Merge Sort Comparisons: " << mergeComparisons << "\n";
    perf << "Bubble Sort Comparisons: " << bubbleComparisons << "\n";
    perf << "Total Swaps: " << swapsCount << "\n";

    delete a; delete b; delete c; delete d;
    return 0;
}
