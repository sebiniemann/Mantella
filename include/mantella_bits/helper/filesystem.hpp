namespace mant {
  inline bool fileExists(
      const std::string& filepath) noexcept;
  
  //
  // Implementation
  //
  
  inline bool fileExists(
      const std::string& filepath) noexcept {
    if (FILE* file = fopen(filepath.c_str(), "r")) {
      fclose(file);
      return true;
    } else {
      return false;
    }
  }
}
