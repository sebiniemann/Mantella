template <typename T>
T predict(
    const std::vector<T>& parameter,
    const std::vector<std::vector<std::vector<T>>>& model);

//
// Implementation
//

template <typename T>
T predict(
    const std::vector<T>& parameter,
    const std::vector<std::vector<std::vector<T>>>& model) {
  assert(!model.empty());
  // TODO Check that the number of weigths per layer is equal to the number of nodes per layer

  std::vector<T> inputs = parameter;
  for (const auto& layer : model) {
    std::vector<T> outputs(layer.size(), T(0.0));
    std::transform(
      layer.cbegin(), layer.cend(),
      outputs.begin(),
      [&inputs](const auto& weights) {
        return std::max(T(0.0), std::inner_product(
          inputs.cbegin(), inputs.cend(),
          weights.cbegin(),
          T(0.0)));
      });

    inputs = outputs;
  }

  return inputs.front();
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("predict", "[helper][predict]") {
  std::vector<std::vector<std::vector<double>>> model = {
    {{2.0, 3.0}, {1.0, 1.0}, {-2.0, 3.0}},
    {{2.0, 2.0, 2.0}, {1.0, -2.0, 2.0}},
    {{3.0, 3.0}}
  };

  CHECK(mant::predict({1.0, 2.0}, model) == Approx(120.0));
}
#endif
