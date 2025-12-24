#include "database_system.hpp"
#include "database_utilities.hpp"
#include <iostream>
#include <iomanip>

// Demo 1: Query and filter operations
void demo_queries() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 1: Query & Filter Operations    ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    auto db = Generator::createSampleEmployees(10);
    
    std::cout << "All Employees:\n";
    std::cout << db.render(StylePresets::corporate());
    
    // Filter active employees
    auto filtered = Query::filter(db.table(), [](const Row& row) {
        return row.getValue("Active") == "true";
    });
    
    std::cout << "\nActive Employees Only (" << filtered.size() << " found):\n";
    
    Database activeDb;
    for (const auto& col : db.table().columns()) {
        activeDb.addColumn(col.name(), col.type(), col.alignment());
    }
    for (const auto& row : filtered) {
        std::map<std::string, std::string> data;
        for (const auto& pair : row.data()) {
            data[pair.first] = pair.second;
        }
        activeDb.addRow(data);
    }
    
    std::cout << activeDb.render(StylePresets::ocean());
}

// Demo 2: Aggregation functions
void demo_aggregation() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 2: Aggregation Functions        ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    auto db = Generator::createSampleEmployees(10);
    
    // Calculate salary statistics
    double totalSalary = Query::sum(db.table(), "Salary");
    double avgSalary = Query::average(db.table(), "Salary");
    double minSalary = Query::min(db.table(), "Salary");
    double maxSalary = Query::max(db.table(), "Salary");
    
    Database statsDb;
    statsDb.addColumn("Metric", ColumnType::STRING, Alignment::LEFT);
    statsDb.addColumn("Value", ColumnType::STRING, Alignment::RIGHT);
    
    statsDb.addRow({{"Metric", "Total Payroll"}, {"Value", "$" + std::to_string((int)totalSalary)}});
    statsDb.addRow({{"Metric", "Average Salary"}, {"Value", "$" + std::to_string((int)avgSalary)}});
    statsDb.addRow({{"Metric", "Minimum Salary"}, {"Value", "$" + std::to_string((int)minSalary)}});
    statsDb.addRow({{"Metric", "Maximum Salary"}, {"Value", "$" + std::to_string((int)maxSalary)}});
    
    RenderConfig cfg = StylePresets::fire();
    cfg.showFooter = true;
    cfg.footerText = "💰 Financial Summary Report";
    cfg.footerStyle.bold = true;
    
    std::cout << statsDb.render(cfg);
}

// Demo 3: Sorting and transformations
void demo_sorting() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 3: Sorting & Transformations    ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    auto db = Generator::createSampleEmployees(8);
    
    std::cout << "Sorted by Salary (Descending):\n";
    auto sorted = Transform::sortBy(db.table(), "Salary", false);
    
    Database sortedDb;
    for (const auto& col : sorted.columns()) {
        sortedDb.addColumn(col.name(), col.type(), col.alignment());
    }
    for (const auto& row : sorted.rows()) {
        std::map<std::string, std::string> data;
        for (const auto& pair : row.data()) {
            data[pair.first] = pair.second;
        }
        sortedDb.addRow(data);
    }
    
    std::cout << sortedDb.render(StylePresets::matrix());
    
    // Select specific columns
    std::cout << "\nName and Salary Only:\n";
    auto selected = Transform::selectColumns(sorted, {"Name", "Salary"});
    
    Database selectedDb;
    for (const auto& col : selected.columns()) {
        selectedDb.addColumn(col.name(), col.type(), col.alignment());
    }
    for (const auto& row : selected.rows()) {
        std::map<std::string, std::string> data;
        for (const auto& pair : row.data()) {
            data[pair.first] = pair.second;
        }
        selectedDb.addRow(data);
    }
    
    std::cout << selectedDb.render(StylePresets::newspaper());
}

// Demo 4: Grouping operations
void demo_grouping() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 4: Grouping Operations          ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    auto db = Generator::createSampleEmployees(10);
    auto groups = Transform::groupBy(db.table(), "Dept");
    
    std::cout << "Employees grouped by Department:\n\n";
    
    for (const auto& group : groups) {
        std::cout << "Department: " << group.first 
                  << " (" << group.second.size() << " employees)\n";
        
        Database deptDb;
        deptDb.addColumn("Name", ColumnType::STRING, Alignment::LEFT);
        deptDb.addColumn("Salary", ColumnType::INTEGER, Alignment::RIGHT);
        deptDb.addColumn("Active", ColumnType::BOOLEAN, Alignment::CENTER);
        
        for (const auto& row : group.second) {
            deptDb.addRow({
                {"Name", row.getValue("Name")},
                {"Salary", row.getValue("Salary")},
                {"Active", row.getValue("Active")}
            });
        }
        
        RenderConfig cfg;
        cfg.boxChars = Unicode::BoxChars::rounded();
        cfg.padding = 1;
        
        std::cout << deptDb.render(cfg) << "\n";
    }
}

// Demo 5: Statistical analysis
void demo_statistics() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 5: Statistical Analysis         ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    auto db = Generator::createSampleEmployees(10);
    auto stats = Statistics::analyze(db.table(), "Salary");
    auto statsDb = Statistics::createStatsTable(stats, "Employee Salaries");
    
    RenderConfig cfg = StylePresets::elegant();
    cfg.showFooter = true;
    cfg.footerText = "📊 Statistical Analysis Report";
    cfg.footerStyle.foreground = Style::Color::BrightMagenta();
    
    std::cout << statsDb.render(cfg);
}

// Demo 6: Export formats
void demo_export() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 6: Export Formats               ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    auto db = Generator::createSampleSales(5);
    
    // Export to CSV
    Export::toCsv(db.table(), "sales_export.csv");
    std::cout << "✅ Exported to CSV: sales_export.csv\n\n";
    
    // Export to Markdown
    std::cout << "Markdown Output:\n";
    std::cout << "```markdown\n";
    std::cout << Export::toMarkdown(db.table());
    std::cout << "```\n\n";
    
    // HTML export (just show it's available)
    std::cout << "✅ HTML export available via Export::toHtml()\n";
}

// Demo 7: Style presets showcase
void demo_style_presets() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 7: Style Presets Showcase       ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    Database db;
    db.addColumn("Style", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Description", ColumnType::STRING, Alignment::LEFT);
    
    db.addRow({{"Style", "Matrix"}, {"Description", "Hacker aesthetic"}});
    db.addRow({{"Style", "Ocean"}, {"Description", "Cool and calm"}});
    db.addRow({{"Style", "Fire"}, {"Description", "Hot and energetic"}});
    
    std::cout << "Matrix Style:\n";
    std::cout << db.render(StylePresets::matrix()) << "\n";
    
    std::cout << "Ocean Style:\n";
    std::cout << db.render(StylePresets::ocean()) << "\n";
    
    std::cout << "Fire Style:\n";
    std::cout << db.render(StylePresets::fire()) << "\n";
    
    std::cout << "Corporate Style:\n";
    std::cout << db.render(StylePresets::corporate()) << "\n";
}

// Demo 8: Limit and pagination
void demo_pagination() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 8: Pagination & Limiting        ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    auto db = Generator::createSampleEmployees(10);
    
    std::cout << "Top 5 Employees:\n";
    auto limited = Transform::limit(db.table(), 5);
    
    Database limitedDb;
    for (const auto& col : limited.columns()) {
        limitedDb.addColumn(col.name(), col.type(), col.alignment());
    }
    for (const auto& row : limited.rows()) {
        std::map<std::string, std::string> data;
        for (const auto& pair : row.data()) {
            data[pair.first] = pair.second;
        }
        limitedDb.addRow(data);
    }
    
    RenderConfig cfg = StylePresets::elegant();
    cfg.showFooter = true;
    cfg.footerText = "Showing 5 of 10 results";
    
    std::cout << limitedDb.render(cfg);
}

// Demo 9: Complex queries
void demo_complex_queries() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 9: Complex Query Examples       ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    auto db = Generator::createSampleEmployees(10);
    
    // Find high-paid active employees
    auto results = Query::filter(db.table(), [](const Row& row) {
        try {
            int salary = std::stoi(row.getValue("Salary"));
            bool active = row.getValue("Active") == "true";
            return salary > 60000 && active;
        } catch (...) {
            return false;
        }
    });
    
    std::cout << "High-Paid Active Employees (Salary > $60k):\n";
    
    Database resultDb;
    resultDb.addColumn("Name", ColumnType::STRING, Alignment::LEFT);
    resultDb.addColumn("Dept", ColumnType::STRING, Alignment::LEFT);
    resultDb.addColumn("Salary", ColumnType::INTEGER, Alignment::RIGHT);
    
    for (const auto& row : results) {
        resultDb.addRow({
            {"Name", row.getValue("Name")},
            {"Dept", row.getValue("Dept")},
            {"Salary", row.getValue("Salary")}
        });
    }
    
    RenderConfig cfg = StylePresets::fire();
    cfg.showFooter = true;
    cfg.footerText = "🎯 " + std::to_string(results.size()) + " matching records found";
    
    std::cout << resultDb.render(cfg);
}

// Demo 10: Unicode showcase
void demo_unicode_features() {
    std::cout << "\n╔═══════════════════════════════════════╗\n";
    std::cout << "║  DEMO 10: Unicode Features            ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n\n";
    
    Database db;
    db.addColumn("Icon", ColumnType::STRING, Alignment::CENTER);
    db.addColumn("Category", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Status", ColumnType::STRING, Alignment::CENTER);
    
    db.addRow({{"Icon", "🎨"}, {"Category", "Design"}, {"Status", "✓"}});
    db.addRow({{"Icon", "💻"}, {"Category", "Development"}, {"Status", "✓"}});
    db.addRow({{"Icon", "📊"}, {"Category", "Analytics"}, {"Status", "⚠"}});
    db.addRow({{"Icon", "🚀"}, {"Category", "Deployment"}, {"Status", "⏳"}});
    db.addRow({{"Icon", "🔒"}, {"Category", "Security"}, {"Status", "✗"}});
    
    RenderConfig cfg;
    cfg.boxChars = Unicode::BoxChars::rounded();
    cfg.headerStyle.foreground = Style::Color::BrightMagenta();
    cfg.headerStyle.bold = true;
    cfg.padding = 2;
    cfg.showFooter = true;
    cfg.footerText = "🌟 Project Status Dashboard";
    cfg.footerStyle.foreground = Style::Color::BrightYellow();
    
    std::cout << db.render(cfg);
}

int main() {
    std::cout << "\n";
    std::cout << "╔═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                             ║\n";
    std::cout << "║     🚀 Advanced Database System - Feature Demonstration     ║\n";
    std::cout << "║                                                             ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════════╝\n";
    
    demo_queries();
    demo_aggregation();
    demo_sorting();
    demo_grouping();
    demo_statistics();
    demo_export();
    demo_style_presets();
    demo_pagination();
    demo_complex_queries();
    demo_unicode_features();
    
    std::cout << "\n";
    std::cout << "╔═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  ✨ All advanced demos completed successfully!              ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << "📝 Features Demonstrated:\n";
    std::cout << "   • Query and filtering\n";
    std::cout << "   • Aggregation functions (sum, avg, min, max)\n";
    std::cout << "   • Sorting and transformations\n";
    std::cout << "   • Grouping operations\n";
    std::cout << "   • Statistical analysis\n";
    std::cout << "   • Export to CSV, HTML, Markdown\n";
    std::cout << "   • Multiple style presets\n";
    std::cout << "   • Pagination and limiting\n";
    std::cout << "   • Complex query predicates\n";
    std::cout << "   • Full Unicode support\n\n";
    
    return 0;
}

// Compilation:
// g++ -std=c++11 -o advanced_demo advanced_demo.cpp
// ./advanced_demo