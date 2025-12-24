#include "Database.hpp"
#include "colors.hpp"
#include "csv.hpp"
#include <iostream>
#include <sstream> // added for C++98-safe number -> string conversion
#include <string>

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << (argc ? argv[0] : "demo_csv") << " <path-to-csv>\n";
            return 1;
        }
        const std::string path = argv[1];
        Database db;
        db.loadFromCsv(path, true);

        // Adjust alignments for readability
        std::vector<Column>& cols = db.table().columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            const std::string& name = cols[i].name();
            if (name == "ID" || name == "Score") cols[i] = Column(name, cols[i].type(), Alignment::RIGHT);
            else if (name == "Emoji" || name == "Active") cols[i] = Column(name, cols[i].type(), Alignment::CENTER);
            else cols[i] = Column(name, cols[i].type(), Alignment::LEFT);
        }

        // Configure rendering with RGB-inspired colors (approximate via Style::Color::RGB)
        RenderConfig cfg = RenderConfig::elegant();
        cfg.padding = 1;
        cfg.showFooter = true;

        // header: warm gradient-ish via RGB approx
        cfg.headerStyle.foreground = Style::Color::RGB(255, 215, 64); // warm light
        cfg.headerStyle.background = Style::Color::RGB(20, 30, 40);  // dark slate
        cfg.headerStyle.bold = true;

        // cells: subtle default foreground, no background
        cfg.cellStyle.foreground = Style::Color::Default();
        cfg.cellStyle.background = Style::Color(49);
        cfg.cellStyle.bold = false;

        // footer
        {
            std::ostringstream oss;
            oss << "Loaded " << db.table().rowCount() << " rows × " << db.table().columnCount() << " columns — demo CSV with Unicode & emoji";
            cfg.footerText = oss.str();
            cfg.footerStyle.foreground = Style::Color::RGB(180,180,255);
            cfg.footerStyle.bold = true;
        }

        // Print rendered table
        std::cout << db.render(cfg) << std::endl;

        // Additionally show CSV::Document demo: raw parse + aggregate sample
        {
            CSV::Document doc;
            if (doc.load(path)) {
                std::cout << "\nSimple CSV stats (using CSV::Aggregate):\n";
                std::cout << " - Rows: " << doc.rowCount() << "\n";
                double avg = CSV::Aggregate::average(doc, "Score");
                std::cout << " - Average Score: " << avg << "\n";
            } else {
                std::cerr << "CSV load error: " << doc.error() << "\n";
            }
        }

        // Test runner for RenderConfig variations
        {
            std::cout << "\n--- Default render ---\n";
            std::cout << db.render();

            // 1) Header color + heavy border
            RenderConfig cfg1 = RenderConfig::elegant();
            cfg1.boxChars = Unicode::BoxChars::heavy();
            cfg1.headerStyle.foreground = Style::Color::BrightMagenta();
            cfg1.headerStyle.background = Style::Color::Default();
            cfg1.borderStyle.foreground = Style::Color::BrightBlue();
            std::cout << "\n--- Header colored, border bright blue ---\n";
            std::cout << db.render(cfg1);

            // 2) Cell foreground + border + header background
            RenderConfig cfg2 = RenderConfig::elegant();
            cfg2.cellStyle.foreground = Style::Color::BrightGreen();
            cfg2.headerStyle.background = Style::Color(44); // cyan bg
            cfg2.borderStyle.foreground = Style::Color::BrightYellow();
            cfg2.boxChars = Unicode::BoxChars::doubleLine();
            std::cout << "\n--- Cell fg bright green, header bg, border bright yellow ---\n";
            std::cout << db.render(cfg2);

            // 3) Footer and padding demo
            RenderConfig cfg3 = RenderConfig::elegant();
            cfg3.showFooter = true;
            // build footer text without std::to_string (C++98)
            std::ostringstream footer_ss;
            footer_ss << "Loaded " << db.count()
                      << " rows × " << db.table().columnCount()
                      << " columns — demo CSV with Unicode & emoji";
            cfg3.footerText = footer_ss.str();
            cfg3.borderStyle.foreground = Style::Color::BrightCyan();
            cfg3.boxChars = Unicode::BoxChars::doubleLine();
            cfg3.padding = 1;
            std::cout << "\n--- Footer, border cyan ---\n";
            std::cout << db.render(cfg3);
        }

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}