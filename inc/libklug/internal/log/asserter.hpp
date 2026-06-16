#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "logger.hpp"

namespace internal::log {

// Neue Hilfsklasse für das Streamen bei einem fehlgeschlagenen Assert
class Asserter {
public:
  // Wir übergeben Datei, Zeile und die fehlgeschlagene Bedingung für ein
  // schönes Log
  Asserter(char const* file, int line, char const* expression) {
    _stream << "[ASSERT FAILED] (" << file << ":" << line << ") Condition '"
            << expression << "' failed. Message: ";
  }

  // Der universelle Stream-Operator, der alles schluckt (Ints, Strings, etc.)
  template<typename T>
  constexpr Asserter& operator<<(T const& value) {
    _stream << value;
    return *this;
  }

  // Im Destruktor wird die Nachricht geloggt und das Programm hart beendet
  ~Asserter() {
    // 1. Auf den normalen Logger-Kanal werfen (Fatal Level)
    Logger::get().log(Level::Critical, _stream.str());

    // 2. Programm sofort und kontrolliert beenden
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::abort();
  }

private:
  std::stringstream _stream;
};

} // namespace internal::log

// --- DAS ASSERT MAKRO ---
// Wenn 'expr' wahr ist, wird der 'else'-Zweig komplett übersprungen.
// Wenn 'expr' falsch ist, wird der Asserter erzeugt und man kann per
// << Text anhängen.
#define LIBKLUG_ASSERT(expr)                                                   \
  if (static_cast<bool>(expr)) [[likely]]                                      \
    void(0);                                                                   \
  else internal::log::Asserter(__FILE__, __LINE__, #expr)
