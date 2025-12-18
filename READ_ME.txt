STUDENT PERFORMANCE ANALYTICS SYSTEM
===================================

1. Overview
-----------
This program processes student academic records stored in a text file.
It uses dynamic memory allocation and singly linked lists to store and
analyze student data across multiple dimensions such as CGPA, enrollment
year, department, and performance tiers.

2. Sorting Algorithms Used
--------------------------
a) Merge Sort
   - Used for ranking students by CGPA.
   - Chosen because it is efficient for linked lists and does not require
     random access.
   - Time Complexity: O(n log n)

b) Bubble Sort
   - Used for sorting students by enrollment year.
   - Chosen for simplicity and clarity.
   - Time Complexity: O(n²)

3. How to Compile and Run
------------------------
Compilation:
g++ main.cpp -o student_analytics

Execution:
./student_analytics

Ensure that "students_data.txt" is present in the same directory.

4. Assumptions Made
-------------------
- Student IDs are intended to be unique.
- Names may contain spaces.
- CGPA range is 0.00 to 4.00.
- Semester range is 1 to 8.
- Credit hours above 250 are considered suspicious.
- All input/output is file-based (no console output).

5. Output Files Generated
-------------------------
- ranked_by_cgpa.txt
- sorted_by_enrollment.txt
- department_analysis.txt
- performance_tiers.txt
- data_issues.txt
- duplicates_report.txt
- algorithm_performance.txt
- search_results.txt

6. Complexity Analysis
---------------------
Merge Sort:
- Best/Average/Worst Case: O(n log n)

Bubble Sort:
- Best Case: O(n)
- Average/Worst Case: O(n²)

Space Complexity:
- O(n) due to linked list storage and list duplication.
