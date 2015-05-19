namespace mant {
  template <typename T>
  class ContinuityAnalysis : public PassivePropertyAnalysis<T> {
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;
  };
}
