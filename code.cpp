#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
 
// product class
class Product
{
public:
    string name;
    string type;
    int devCycles;
    bool isReady;
    bool isLaunched;
    int revenue;
    int assignedEmployees;
 
    Product()
    {
        devCycles = 2;
        isReady = false;
        isLaunched = false;
        revenue = 0;
        assignedEmployees = 0;
    }
 
    Product(string n, string t, int emp)
    {
        name = n;
        type = t;
        assignedEmployees = emp;
        devCycles = 2;
        isReady = false;
        isLaunched = false;
        revenue = 0;
    }
};
 
// base class
class Company
{
protected:
    string name;
    int budget;
    int employees;
 
public:
    Company()
    {
        budget = 0;
        employees = 0;
    }
 
    Company(string n, int b)
    {
        name = n;
        budget = b;
        employees = 0;
    }
 
    string getName() { return name; }
    int getBudget() { return budget; }
    int getEmployees() { return employees; }
 
    void setBudget(int b) { budget = b; }
    void setEmployees(int e) { employees = e; }
};
 
// derived class
class Startup : public Company
{
public:
    int cycle;
    vector<Product> products;
 
    Startup()
    {
        cycle = 1;
    }
 
    Startup(string n, int b) : Company(n, b)
    {
        cycle = 1;
    }
 
    // ─── FILE HANDLING ───────────────────────────────────────────────────────
 
    // Save all startup data to <name>.txt
    void saveToFile()
    {
        ofstream fout(name + ".txt");
        if (!fout)
        {
            cout << "Error: Could not save data for " << name << "\n";
            return;
        }
 
        // Write basic info
        fout << name << "\n";
        fout << budget << "\n";
        fout << employees << "\n";
        fout << cycle << "\n";
 
        // Write products
        fout << products.size() << "\n";
        for (int i = 0; i < (int)products.size(); i++)
        {
            fout << products[i].name << "\n";
            fout << products[i].type << "\n";
            fout << products[i].devCycles << "\n";
            fout << products[i].isReady << "\n";
            fout << products[i].isLaunched << "\n";
            fout << products[i].revenue << "\n";
            fout << products[i].assignedEmployees << "\n";
        }
 
        fout.close();
    }
 
    // Load startup data from <sname>.txt
    bool loadFromFile(string sname)
    {
        ifstream fin(sname + ".txt");
        if (!fin)
            return false;
 
        int numProducts;
        fin >> name >> budget >> employees >> cycle >> numProducts;
 
        products.clear();
        for (int i = 0; i < numProducts; i++)
        {
            Product p;
            fin >> p.name >> p.type >> p.devCycles
                >> p.isReady >> p.isLaunched
                >> p.revenue >> p.assignedEmployees;
            products.push_back(p);
        }
 
        fin.close();
        return true;
    }
 
    // ─────────────────────────────────────────────────────────────────────────
 
    void hireEmployees()
    {
        int count;
        cout << "\nEnter employees to hire: ";
        cin >> count;
 
        if (count <= 0)
        {
            cout << "Invalid input\n";
            return;
        }
 
        int cost = count * 50000;
 
        if (cost > budget)
        {
            cout << "Not enough money\n";
            return;
        }
 
        budget -= cost;
        employees += count;
 
        cout << "Employees hired\n";
        cout << "Remaining money: Rs " << budget << endl;
 
        saveToFile(); // update file
    }
 
    void createProduct()
    {
        if (employees == 0)
        {
            cout << "No employees available\n";
            return;
        }
 
        string pname, ptype;
        int emp;
 
        cout << "\nEnter product name: ";
        cin >> pname;
 
        cout << "1. App\n2. Hardware\n3. Service\nChoice: ";
        int ch;
        cin >> ch;
 
        if (ch == 1)
            ptype = "App";
        else if (ch == 2)
            ptype = "Hardware";
        else
            ptype = "Service";
 
        cout << "Enter employees to assign: ";
        cin >> emp;
 
        if (emp <= 0 || emp > employees)
        {
            cout << "Invalid number of employees\n";
            return;
        }
 
        int cost = 100000;
 
        if (budget < cost)
        {
            cout << "Not enough money\n";
            return;
        }
 
        budget -= cost;
        products.push_back(Product(pname, ptype, emp));
 
        cout << "Product created\n";
 
        saveToFile(); // update file
    }
 
    void launchProduct()
    {
        bool found = false;
 
        for (int i = 0; i < (int)products.size(); i++)
        {
            if (products[i].isReady && !products[i].isLaunched)
            {
                found = true;
            }
        }
 
        if (!found)
        {
            cout << "No product ready\n";
            return;
        }
 
        cout << "\nReady products:\n";
 
        vector<int> idxList;
        int num = 1;
 
        for (int i = 0; i < (int)products.size(); i++)
        {
            if (products[i].isReady && !products[i].isLaunched)
            {
                cout << num++ << ". " << products[i].name << endl;
                idxList.push_back(i);
            }
        }
 
        int ch;
        cin >> ch;
        ch--;
 
        if (ch < 0 || ch >= (int)idxList.size())
        {
            cout << "Invalid choice\n";
            return;
        }
 
        Product &p = products[idxList[ch]];
        p.isLaunched = true;
 
        if (p.type == "App")
            p.revenue = p.assignedEmployees * 80000;
        else if (p.type == "Hardware")
            p.revenue = p.assignedEmployees * 120000;
        else
            p.revenue = p.assignedEmployees * 60000;
 
        budget += p.revenue;
 
        cout << "Product launched\n";
        cout << "Earned Rs " << p.revenue << endl;
 
        saveToFile(); // update file
    }
 
    void nextCycle()
    {
        cout << "\nNext cycle...\n";
 
        int salary = employees * 30000;
        budget -= salary;
 
        cout << "Salary paid: Rs " << salary << endl;
 
        for (int i = 0; i < (int)products.size(); i++)
        {
            if (!products[i].isReady && !products[i].isLaunched)
            {
                products[i].devCycles--;
 
                if (products[i].devCycles <= 0)
                {
                    products[i].isReady = true;
                    cout << products[i].name << " is ready\n";
                }
            }
 
            if (products[i].isLaunched)
            {
                budget += products[i].revenue;
                cout << products[i].name << " earned Rs " << products[i].revenue << endl;
            }
        }
 
        cycle++;
        cout << "Budget: Rs " << budget << endl;
 
        saveToFile(); // update file
    }
 
    void viewProducts()
    {
        if (products.empty())
        {
            cout << "No products\n";
            return;
        }
 
        for (int i = 0; i < (int)products.size(); i++)
        {
            cout << "\n" << products[i].name << " (" << products[i].type << ")" << endl;
            cout << "Employees: " << products[i].assignedEmployees << endl;
 
            if (products[i].isLaunched)
                cout << "Launched (Rs " << products[i].revenue << "/cycle)\n";
            else if (products[i].isReady)
                cout << "Ready\n";
            else
                cout << "Development (" << products[i].devCycles << " left)\n";
        }
    }
 
    void viewStatus()
    {
        cout << "\nStartup: " << name << endl;
        cout << "Budget: Rs " << budget << endl;
        cout << "Employees: " << employees << endl;
        cout << "Cycle: " << cycle << endl;
    }
 
    void showExpenses()
    {
        cout << "\n===== EXPENSES =====\n";
        cout << "Hiring cost approx: Rs " << employees * 50000 << endl;
        cout << "Salary per cycle: Rs " << employees * 30000 << endl;
    }
};
 
// ─── INDEX FILE: tracks all startup names ────────────────────────────────────
 
void saveIndex(vector<string> &names)
{
    ofstream fout("startups_index.txt");
    fout << names.size() << "\n";
    for (int i = 0; i < (int)names.size(); i++)
        fout << names[i] << "\n";
    fout.close();
}
 
vector<string> loadIndex()
{
    vector<string> names;
    ifstream fin("startups_index.txt");
    if (!fin)
        return names;
    int sz;
    fin >> sz;
    for (int i = 0; i < sz; i++)
    {
        string n;
        fin >> n;
        names.push_back(n);
    }
    fin.close();
    return names;
}
 
// ─────────────────────────────────────────────────────────────────────────────
 
// menus
void showMainMenu()
{
    cout << "\n===== STARTUP SIMULATOR =====\n";
    cout << "1 Create Startup\n";
    cout << "2 Select Startup\n";
    cout << "3 View All\n";
    cout << "4 Merge Startups\n";
    cout << "5 Exit\n";
}
 
void showStartupMenu()
{
    cout << "\n===== STARTUP MENU =====\n";
    cout << "1 Hire Employees\n";
    cout << "2 Create Product\n";
    cout << "3 Launch Product\n";
    cout << "4 View Products\n";
    cout << "5 Next Cycle\n";
    cout << "6 View Status\n";
    cout << "7 View Expenses\n";
    cout << "8 Back\n";
}
 
// merge
void mergeStartups(vector<Startup> &startups, vector<string> &index)
{
    if (startups.size() < 2)
    {
        cout << "Need at least 2 startups\n";
        return;
    }
 
    for (int i = 0; i < (int)startups.size(); i++)
        cout << i + 1 << ". " << startups[i].getName() << endl;
 
    int a, b;
    cout << "Enter two indices: ";
    cin >> a >> b;
    a--;
    b--;
 
    if (a < 0 || b < 0 || a >= (int)startups.size() || b >= (int)startups.size() || a == b)
    {
        cout << "Invalid input\n";
        return;
    }
 
    // Merge b into a
    startups[a].setBudget(startups[a].getBudget() + startups[b].getBudget());
    startups[a].setEmployees(startups[a].getEmployees() + startups[b].getEmployees());
 
    for (int i = 0; i < (int)startups[b].products.size(); i++)
        startups[a].products.push_back(startups[b].products[i]);
 
    // Save merged startup, delete old file of b
    startups[a].saveToFile();
    remove((startups[b].getName() + ".txt").c_str());
 
    // Remove b from index and vector
    index.erase(index.begin() + b);
    startups.erase(startups.begin() + b);
 
    saveIndex(index); // update index file
 
    cout << "Merged successfully\n";
}
 
// main
int main()
{
    vector<Startup> startups;
    vector<string> index = loadIndex(); // load saved startup names
 
    // Load all previously saved startups from their files
    for (int i = 0; i < (int)index.size(); i++)
    {
        Startup s;
        if (s.loadFromFile(index[i]))
            startups.push_back(s);
    }
 
    int choice;
 
    do
    {
        showMainMenu();
        cout << "Enter choice: ";
        cin >> choice;
 
        if (choice == 1)
        {
            string sname;
            cout << "Enter name: ";
            cin >> sname;
 
            // Check if name already exists
            bool exists = false;
            for (int i = 0; i < (int)startups.size(); i++)
            {
                if (startups[i].getName() == sname)
                {
                    exists = true;
                    break;
                }
            }
 
            if (exists)
            {
                cout << "Startup with this name already exists\n";
                continue;
            }
 
            Startup s(sname, 500000);
            s.saveToFile(); // create file immediately on startup creation
 
            index.push_back(sname);
            saveIndex(index); // update index file
 
            startups.push_back(s);
 
            cout << "\nStartup created successfully\n";
            cout << "Name: " << sname << endl;
            cout << "Initial Budget: Rs 500000\n";
            cout << "Employees: 0\n";
            cout << "Cycle: 1\n";
            cout << "(Data saved to " << sname << ".txt)\n";
        }
 
        else if (choice == 2)
        {
            if (startups.empty())
            {
                cout << "No startups\n";
                continue;
            }
 
            for (int i = 0; i < (int)startups.size(); i++)
                cout << i + 1 << ". " << startups[i].getName() << endl;
 
            int idx;
            cin >> idx;
            idx--;
 
            if (idx < 0 || idx >= (int)startups.size())
            {
                cout << "Invalid choice\n";
                continue;
            }
 
            int ch;
 
            do
            {
                showStartupMenu();
                cout << "Enter choice: ";
                cin >> ch;
 
                if (ch == 1)
                    startups[idx].hireEmployees();
                else if (ch == 2)
                    startups[idx].createProduct();
                else if (ch == 3)
                    startups[idx].launchProduct();
                else if (ch == 4)
                    startups[idx].viewProducts();
                else if (ch == 5)
                    startups[idx].nextCycle();
                else if (ch == 6)
                    startups[idx].viewStatus();
                else if (ch == 7)
                    startups[idx].showExpenses();
 
            } while (ch != 8);
        }
 
        else if (choice == 3)
        {
            if (startups.empty())
            {
                cout << "No startups yet\n";
                continue;
            }
            for (int i = 0; i < (int)startups.size(); i++)
                startups[i].viewStatus();
        }
 
        else if (choice == 4)
        {
            mergeStartups(startups, index);
        }
 
    } while (choice != 5);
 
    return 0;
}