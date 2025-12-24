#include "Database.hpp"
#include "colors.hpp"
#include "csv.hpp"
#include <iostream>
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

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}