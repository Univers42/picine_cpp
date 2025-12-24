#include "database_system.hpp"
#include <iostream>

// Example 1: Basic table creation and rendering
void example_basic() {
    std::cout << "\n=== EXAMPLE 1: Basic Table ===\n\n";
    
    Database db;
    
    // Add columns with different types and alignments
    db.addColumn("ID", ColumnType::INTEGER, Alignment::RIGHT);
    db.addColumn("Name", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Score", ColumnType::DOUBLE, Alignment::RIGHT);
    db.addColumn("Active", ColumnType::BOOLEAN, Alignment::CENTER);
    
    // Add data
    db.addRow({{"ID", "1"}, {"Name", "Alice"}, {"Score", "95.5"}, {"Active", "true"}});
    db.addRow({{"ID", "2"}, {"Name", "Bob"}, {"Score", "87.3"}, {"Active", "true"}});
    db.addRow({{"ID", "3"}, {"Name", "Charlie"}, {"Score", "92.1"}, {"Active", "false"}});
    db.addRow({{"ID", "4"}, {"Name", "Diana"}, {"Score", "88.7"}, {"Active", "true"}});
    
    // Render with default config
    std::cout << db.render();
}

// Example 2: Elegant style with custom colors
void example_styled() {
    std::cout << "\n=== EXAMPLE 2: Elegant Styled Table ===\n\n";
    
    Database db;
    
    db.addColumn("Product", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Price", ColumnType::DOUBLE, Alignment::RIGHT);
    db.addColumn("Stock", ColumnType::INTEGER, Alignment::RIGHT);
    db.addColumn("Status", ColumnType::STRING, Alignment::CENTER);
    
    db.addRow({{"Product", "Laptop"}, {"Price", "$999.99"}, {"Stock", "15"}, {"Status", "✓"}});
    db.addRow({{"Product", "Mouse"}, {"Price", "$29.99"}, {"Stock", "150"}, {"Status", "✓"}});
    db.addRow({{"Product", "Keyboard"}, {"Price", "$79.99"}, {"Stock", "45"}, {"Status", "✓"}});
    db.addRow({{"Product", "Monitor"}, {"Price", "$349.99"}, {"Stock", "8"}, {"Status", "⚠"}});
    
    // Custom elegant configuration
    RenderConfig config = RenderConfig::elegant();
    config.showFooter = true;
    config.footerText = "Total Items: 4 | Total Value: $1,459.96";
    config.footerStyle.foreground = Style::Color::BrightYellow();
    config.footerStyle.italic = true;
    
    std::cout << db.render(config);
}

// Example 3: Heavy borders with custom styling
void example_heavy() {
    std::cout << "\n=== EXAMPLE 3: Heavy Borders ===\n\n";
    
    Database db;
    
    db.addColumn("Rank", ColumnType::INTEGER, Alignment::CENTER);
    db.addColumn("Player", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Score", ColumnType::INTEGER, Alignment::RIGHT);
    
    db.addRow({{"Rank", "🥇"}, {"Player", "Champion"}, {"Score", "9500"}});
    db.addRow({{"Rank", "🥈"}, {"Player", "Runner-up"}, {"Score", "8900"}});
    db.addRow({{"Rank", "🥉"}, {"Player", "Third Place"}, {"Score", "8200"}});
    
    RenderConfig config;
    config.boxChars = Unicode::BoxChars::heavy();
    config.headerStyle.foreground = Style::Color::BrightMagenta();
    config.headerStyle.bold = true;
    config.cellStyle.foreground = Style::Color::BrightWhite();
    config.padding = 2;
    
    std::cout << db.render(config);
}

// Example 4: Unicode characters and international text
void example_unicode() {
    std::cout << "\n=== EXAMPLE 4: Unicode Support ===\n\n";
    
    Database db;
    
    db.addColumn("País", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Ciudad", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Población", ColumnType::STRING, Alignment::RIGHT);
    
    db.addRow({{"País", "日本"}, {"Ciudad", "東京"}, {"Población", "13.96M"}});
    db.addRow({{"País", "España"}, {"Ciudad", "Madrid"}, {"Población", "3.27M"}});
    db.addRow({{"País", "中国"}, {"Ciudad", "上海"}, {"Población", "24.28M"}});
    db.addRow({{"País", "Россия"}, {"Ciudad", "Москва"}, {"Población", "12.54M"}});
    
    RenderConfig config;
    config.boxChars = Unicode::BoxChars::rounded();
    config.headerStyle.foreground = Style::Color::BrightCyan();
    config.headerStyle.bold = true;
    
    std::cout << db.render(config);
}

// Example 5: Loading from CSV (simulated)
void example_csv() {
    std::cout << "\n=== EXAMPLE 5: CSV Loading ===\n\n";
    
    // Create a sample CSV file first
    std::ofstream csvFile("sample_data.csv");
    csvFile << "Employee,Department,Salary,Hire_Date\n";
    csvFile << "John Smith,Engineering,95000,2020-01-15\n";
    csvFile << "Sarah Johnson,Marketing,78000,2019-06-22\n";
    csvFile << "Michael Brown,Sales,82000,2021-03-10\n";
    csvFile << "Emma Davis,HR,71000,2018-11-05\n";
    csvFile.close();
    
    try {
        Database db;
        db.loadFromCsv("sample_data.csv", true);
        
        // Customize column alignments after loading
        auto& cols = db.table().columns();
        if (cols.size() >= 3) {
            cols[2] = Column("Salary", ColumnType::DOUBLE, Alignment::RIGHT);
        }
        
        RenderConfig config = RenderConfig::elegant();
        config.showFooter = true;
        config.footerText = "🏢 Company Employee Database";
        config.footerStyle.foreground = Style::Color::BrightGreen();
        
        std::cout << db.render(config);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Example 6: Query operations
void example_queries() {
    std::cout << "\n=== EXAMPLE 6: Query Operations ===\n\n";
    
    Database db;
    
    db.addColumn("ID", ColumnType::INTEGER, Alignment::RIGHT);
    db.addColumn("Department", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Budget", ColumnType::STRING, Alignment::RIGHT);
    
    db.addRow({{"ID", "1"}, {"Department", "Engineering"}, {"Budget", "$500K"}});
    db.addRow({{"ID", "2"}, {"Department", "Marketing"}, {"Budget", "$300K"}});
    db.addRow({{"ID", "3"}, {"Department", "Engineering"}, {"Budget", "$450K"}});
    db.addRow({{"ID", "4"}, {"Department", "Sales"}, {"Budget", "$400K"}});
    db.addRow({{"ID", "5"}, {"Department", "Engineering"}, {"Budget", "$550K"}});
    
    // Query for Engineering departments
    auto results = db.where("Department", "Engineering");
    
    std::cout << "Found " << results.size() << " Engineering departments:\n\n";
    
    // Create a new database with filtered results
    Database filteredDb;
    filteredDb.addColumn("ID", ColumnType::INTEGER, Alignment::RIGHT);
    filteredDb.addColumn("Department", ColumnType::STRING, Alignment::LEFT);
    filteredDb.addColumn("Budget", ColumnType::STRING, Alignment::RIGHT);
    
    for (const auto& row : results) {
        std::map<std::string, std::string> data;
        for (const auto& pair : row.data()) {
            data[pair.first] = pair.second;
        }
        filteredDb.addRow(data);
    }
    
    RenderConfig config;
    config.headerStyle.foreground = Style::Color::BrightGreen();
    config.headerStyle.bold = true;
    
    std::cout << filteredDb.render(config);
}

// Example 7: Minimal style
void example_minimal() {
    std::cout << "\n=== EXAMPLE 7: Minimal Style ===\n\n";
    
    Database db;
    
    db.addColumn("Task", ColumnType::STRING, Alignment::LEFT);
    db.addColumn("Done", ColumnType::BOOLEAN, Alignment::CENTER);
    
    db.addRow({{"Task", "Design database schema"}, {"Done", "true"}});
    db.addRow({{"Task", "Implement rendering"}, {"Done", "true"}});
    db.addRow({{"Task", "Add CSV support"}, {"Done", "true"}});
    db.addRow({{"Task", "Write documentation"}, {"Done", "false"}});
    
    RenderConfig config = RenderConfig::minimal();
    config.boxChars = Unicode::BoxChars::doubleLine();
    
    std::cout << db.render(config);
}

// Example 8: Financial report style
void example_financial() {
    std::cout << "\n=== EXAMPLE 8: Financial Report ===\n\n";
    
    Database db;
    
    db.addColumn("Quarter", ColumnType::STRING, Alignment::CENTER);
    db.addColumn("Revenue", ColumnType::STRING, Alignment::RIGHT);
    db.addColumn("Expenses", ColumnType::STRING, Alignment::RIGHT);
    db.addColumn("Profit", ColumnType::STRING, Alignment::RIGHT);
    db.addColumn("Δ", ColumnType::STRING, Alignment::RIGHT);
    
    db.addRow({{"Quarter", "Q1 2024"}, {"Revenue", "$2.5M"}, {"Expenses", "$1.8M"}, {"Profit", "$700K"}, {"Δ", "+15%"}});
    db.addRow({{"Quarter", "Q2 2024"}, {"Revenue", "$2.8M"}, {"Expenses", "$1.9M"}, {"Profit", "$900K"}, {"Δ", "+28%"}});
    db.addRow({{"Quarter", "Q3 2024"}, {"Revenue", "$3.1M"}, {"Expenses", "$2.0M"}, {"Profit", "$1.1M"}, {"Δ", "+22%"}});
    db.addRow({{"Quarter", "Q4 2024"}, {"Revenue", "$3.4M"}, {"Expenses", "$2.1M"}, {"Profit", "$1.3M"}, {"Δ", "+18%"}});
    
    RenderConfig config;
    config.boxChars = Unicode::BoxChars::doubleLine();
    config.headerStyle.foreground = Style::Color::BrightWhite();
    config.headerStyle.background = Style::Color{42}; // Green background
    config.headerStyle.bold = true;
    config.showFooter = true;
    config.footerText = "📊 Annual Growth: +21% | Total Profit: $4.0M";
    config.footerStyle.foreground = Style::Color::BrightYellow();
    config.footerStyle.bold = true;
    config.padding = 2;
    
    std::cout << db.render(config);
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║   Database & Table Rendering System - Demo Suite      ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n";
    
    example_basic();
    example_styled();
    example_heavy();
    example_unicode();
    example_csv();
    example_queries();
    example_minimal();
    example_financial();
    
    std::cout << "\n✨ All examples completed successfully!\n\n";
    
    return 0;
}

// Compilation instructions:
// g++ -std=c++11 -o database_demo example_usage.cpp
// ./database_demo
//
// For proper Unicode display, ensure your terminal supports UTF-8 encoding