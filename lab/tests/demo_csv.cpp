#include "colors.hpp"
#include "csv.hpp"
#include "Date.hpp"
#include "database_system.hpp"
#include <iostream>
#include <fstream>

// ============================================================================
// EXAMPLE 1: CSV Processing with Filtering
// ============================================================================

void example_csv_filtering() {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  Example 1: CSV Processing & Filtering      ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";
    
    // Create sample CSV
    std::ofstream file("employees.csv");
    file << "ID,Name,Department,Salary,HireDate,Active\n";
    file << "1,Alice Johnson,Engineering,95000,2020-01-15,true\n";
    file << "2,Bob Smith,Marketing,75000,2019-06-22,true\n";
    file << "3,Charlie Brown,Engineering,87000,2021-03-10,false\n";
    file << "4,Diana Prince,Sales,82000,2018-11-05,true\n";
    file << "5,Eve Davis,Engineering,92000,2020-08-17,true\n";
    file.close();
    
    // Load and process
    CSV::Document doc;
    if (!doc.load("employees.csv")) {
        std::cerr << "Error: " << doc.error() << "\n";
        return;
    }
    
    std::cout << "Total employees: " << doc.rowCount() << "\n\n";
    
    // Filter: Only Engineering department
    auto engineers = CSV::Filter::where(doc, "Department", "Engineering");
    std::cout << "Engineers found: " << engineers.rowCount() << "\n";
    
    // Calculate average salary for engineers
    double avgSalary = CSV::Aggregate::average(engineers, "Salary");
    std::cout << "Average Engineering salary: $" << (int)avgSalary << "\n\n";
    
    // Filter: Active employees with salary > 80000
    auto highEarners = CSV::Filter::filter(doc, [](const CSV::Row& row) {
        return row.getBool(5) && row.getDouble(3) > 80000;
    });
    
    std::cout << "High earning active employees:\n";
    for (const auto& row : highEarners.rows()) {
        std::cout << "  - " << row[1] << ": $" << row[3] << "\n";
    }
}

// ============================================================================
// EXAMPLE 2: Date Operations
// ============================================================================

void example_date_operations() {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  Example 2: Date Operations                  ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";
    
    // Parse different date formats
    Date d1 = Date::parse("2024-12-24", "ISO");
    Date d2 = Date::parse("24/12/2024", "DD/MM/YYYY");
    Date d3 = Date::parse("12/24/2024", "MM/DD/YYYY");
    
    std::cout << "Parsed dates:\n";
    std::cout << "  ISO: " << d1.toString() << "\n";
    std::cout << "  DMY: " << d2.toString() << "\n";
    std::cout << "  MDY: " << d3.toString() << "\n\n";
    
    // Date arithmetic
    Date today = Date::today();
    Date nextWeek = today.addDays(7);
    Date nextMonth = today.addMonths(1);
    Date nextYear = today.addYears(1);
    
    std::cout << "Today: " << today.toString("Weekday, Month DD, YYYY") << "\n";
    std::cout << "Next week: " << nextWeek.toString("Month DD, YYYY") << "\n";
    std::cout << "Next month: " << nextMonth.toString("Month DD, YYYY") << "\n";
    std::cout << "Next year: " << nextYear.toString("YYYY-MM-DD") << "\n\n";
    
    // Date calculations
    Date birthday(1990, 5, 15);
    int age = DateUtils::age(birthday, today);
    std::cout << "If born on " << birthday.toString("Month DD, YYYY") 
              << ", age would be: " << age << "\n\n";
    
    // Business days
    Date start(2024, 12, 23);  // Monday
    Date end(2024, 12, 31);    // Tuesday
    int businessDays = DateUtils::businessDaysBetween(start, end);
    std::cout << "Business days between " << start.toString() 
              << " and " << end.toString() << ": " << businessDays << "\n";
}

// ============================================================================
// EXAMPLE 3: Color System
// ============================================================================

void example_colors() {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  Example 3: Color System                     ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";
    
    Colors::Formatter fmt(Colors::Formatter::ANSI_256);
    
    // Basic colors
    std::cout << fmt.fg("Red text", Colors::Palette::Red()) << "\n";
    std::cout << fmt.fg("Green text", Colors::Palette::Green()) << "\n";
    std::cout << fmt.fg("Blue text", Colors::Palette::Blue()) << "\n";
    std::cout << fmt.fg("Cyan text", Colors::Palette::Cyan()) << "\n\n";
    
    // Material Design colors
    std::cout << fmt.fg("Material Red", Colors::Palette::MaterialRed()) << "\n";
    std::cout << fmt.fg("Material Blue", Colors::Palette::MaterialBlue()) << "\n";
    std::cout << fmt.fg("Material Green", Colors::Palette::MaterialGreen()) << "\n\n";
    
    // Text formatting
    std::cout << Colors::Formatter::bold("Bold text") << "\n";
    std::cout << Colors::Formatter::italic("Italic text") << "\n";
    std::cout << Colors::Formatter::underline("Underlined text") << "\n\n";
    
    // Color manipulation
    Colors::RGB base = Colors::Palette::Blue();
    Colors::RGB lighter = base.lighten(0.3);
    Colors::RGB darker = base.darken(0.3);
    
    std::cout << "Base color:    " << fmt.fg("████████", base) << " " << base.toHex() << "\n";
    std::cout << "Lighter (30%): " << fmt.fg("████████", lighter) << " " << lighter.toHex() << "\n";
    std::cout << "Darker (30%):  " << fmt.fg("████████", darker) << " " << darker.toHex() << "\n\n";
    
    // Gradient
    auto gradient = Colors::Gradient::Rainbow().generate(10);
    std::cout << "Rainbow gradient: ";
    for (const auto& color : gradient) {
        std::cout << fmt.fg("█", color);
    }
    std::cout << "\n";
}

// ============================================================================
// EXAMPLE 4: Integration - CSV to Styled Database Table
// ============================================================================

void example_integration() {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  Example 4: Full Integration                 ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";
    
    // Create sample data with dates
    std::ofstream file("sales.csv");
    file << "Date,Product,Units,Revenue\n";
    file << "2024-01-15,Widget A,150,$7500\n";
    file << "2024-02-20,Widget B,200,$10000\n";
    file << "2024-03-10,Widget A,175,$8750\n";
    file << "2024-04-05,Widget C,120,$9600\n";
    file << "2024-05-18,Widget B,220,$11000\n";
    file.close();
    
    // Load CSV
    CSV::Document csvDoc;
    csvDoc.load("sales.csv");
    
    // Convert to Database
    Database db;
    db.addColumn("Date", ColumnType::DATE, Alignment::LEFT);
    db.addColumn("Product", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Units", ColumnType::INTEGER, Alignment::RIGHT);
    db.addColumn("Revenue", ColumnType::STRING, Alignment::RIGHT);
    
    for (const auto& row : csvDoc.rows()) {
        db.addRow({
            {"Date", row[0]},
            {"Product", row[1]},
            {"Units", row[2]},
            {"Revenue", row[3]}
        });
    }
    
    // Calculate totals
    double totalRevenue = CSV::Aggregate::sum(csvDoc, "Revenue");
    int totalUnits = 0;
    for (const auto& row : csvDoc.rows()) {
        totalUnits += row.getInt(2);
    }
    
    // Style with colors
    RenderConfig config;
    config.boxChars = Unicode::BoxChars::doubleLine();
    config.headerStyle.foreground = Style::Color::BrightCyan();
    config.headerStyle.bold = true;
    config.padding = 2;
    config.showFooter = true;
    config.footerText = "Total Units: " + std::to_string(totalUnits) + 
                        " | Total Revenue: $" + std::to_string((int)totalRevenue);
    config.footerStyle.foreground = Style::Color::BrightYellow();
    config.footerStyle.bold = true;
    
    std::cout << db.render(config);
}

// ============================================================================
// EXAMPLE 5: Advanced CSV Analysis
// ============================================================================

void example_advanced_csv() {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  Example 5: Advanced CSV Analysis            ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";
    
    // Load existing data
    CSV::Document doc;
    doc.load("employees.csv");
    
    // Group by department
    auto groups = CSV::Filter::groupBy(doc, "Department");
    
    std::cout << "Department Analysis:\n";
    std::cout << "──────────────────────────────────────────────\n\n";
    
    for (const auto& group : groups) {
        std::cout << "Department: " << group.first << "\n";
        std::cout << "  Employees: " << group.second.rowCount() << "\n";
        std::cout << "  Avg Salary: $" << (int)CSV::Aggregate::average(group.second, "Salary") << "\n";
        std::cout << "  Total Payroll: $" << (int)CSV::Aggregate::sum(group.second, "Salary") << "\n";
        
        // Active employees in this department
        auto active = CSV::Filter::filter(group.second, [](const CSV::Row& row) {
            return row.getBool(5);
        });
        std::cout << "  Active: " << active.rowCount() << "/" << group.second.rowCount() << "\n\n";
    }
    
    // Salary distribution
    double minSalary = CSV::Aggregate::min(doc, "Salary");
    double maxSalary = CSV::Aggregate::max(doc, "Salary");
    double avgSalary = CSV::Aggregate::average(doc, "Salary");
    double stdDev = CSV::Aggregate::stddev(doc, "Salary");
    
    std::cout << "Salary Statistics:\n";
    std::cout << "  Min: $" << (int)minSalary << "\n";
    std::cout << "  Max: $" << (int)maxSalary << "\n";
    std::cout << "  Avg: $" << (int)avgSalary << "\n";
    std::cout << "  StdDev: $" << (int)stdDev << "\n";
}

// ============================================================================
// EXAMPLE 6: Date Range Processing
// ============================================================================

void example_date_range() {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  Example 6: Date Range Processing            ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";
    
    Date start(2024, 12, 1);
    Date end(2024, 12, 31);
    DateRange december(start, end);
    
    std::cout << "December 2024 Analysis:\n";
    std::cout << "  Total days: " << december.days() << "\n";
    
    // Count weekends
    int weekends = 0;
    int weekdays = 0;
    for (const Date& date : december) {
        if (DateUtils::isWeekend(date)) {
            ++weekends;
        } else {
            ++weekdays;
        }
    }
    
    std::cout << "  Weekdays: " << weekdays << "\n";
    std::cout << "  Weekends: " << weekends << "\n\n";
    
    // Find all Mondays
    std::cout << "All Mondays in December:\n";
    for (const Date& date : december) {
        if (date.weekday() == 1) {  // Monday
            std::cout << "  " << date.toString("Weekday, Month DD") << "\n";
        }
    }
}

// ============================================================================
// EXAMPLE 7: Color Gradients in Tables
// ============================================================================

void example_gradient_table() {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  Example 7: Colored Gradient Table           ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";
    
    Database db;
    db.addColumn("Temperature", ColumnType::INTEGER, Alignment::RIGHT);
    db.addColumn("Status", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Color", ColumnType::STRING, Alignment::CENTER);
    
    // Temperature scale with colors
    Colors::Gradient tempGradient(Colors::Palette::Blue(), Colors::Palette::Red());
    Colors::Formatter fmt;
    
    std::vector<int> temps = {0, 25, 50, 75, 100};
    std::vector<std::string> status = {"Freezing", "Cool", "Warm", "Hot", "Boiling"};
    
    for (size_t i = 0; i < temps.size(); ++i) {
        double position = (double)i / (temps.size() - 1);
        Colors::RGB color = tempGradient.at(position);
        
        std::string colorBlock = fmt.fg("████", color);
        
        db.addRow({
            {"Temperature", std::to_string(temps[i]) + "°C"},
            {"Status", status[i]},
            {"Color", colorBlock}
        });
    }
    
    RenderConfig config;
    config.boxChars = Unicode::BoxChars::rounded();
    config.headerStyle.bold = true;
    config.headerStyle.foreground = Style::Color::BrightMagenta();
    
    std::cout << db.render(config);
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                    ║\n";
    std::cout << "║  🎨 Integrated System Demonstration                ║\n";
    std::cout << "║  Colors + CSV + Date + Database                    ║\n";
    std::cout << "║                                                    ║\n";
    std::cout << "╚════════════════════════════════════════════════════╝\n";
    
    try {
        example_csv_filtering();
        example_date_operations();
        example_colors();
        example_integration();
        example_advanced_csv();
        example_date_range();
        example_gradient_table();
        
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════════════╗\n";
        std::cout << "║  ✨ All examples completed successfully!           ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

// Compile with:
// g++ -std=c++11 -o integrated integrated_example.cpp
// ./integrated