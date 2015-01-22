namespace mant {
  inline bool file_exists(const std::string& filepath) {
    if (FILE* file = fopen(filepath.c_str(), "r")) {
      fclose(file);
        return true;
    } else {
      return false;
    }
  }
}
