template <typename T>
std::vector<std::vector<std::vector<T>>> learn(
    const std::vector<std::pair<std::vector<T>, T>>& samples,
    const std::size_t epochs,
    const T acceptable_deviation,
    const T learn_rate,
    std::vector<std::vector<std::vector<T>>> model);

//
// Implementation
//

template <typename T>
std::vector<std::vector<std::vector<T>>> learn(
    const std::vector<std::pair<std::vector<T>, T>>& samples,
    const std::size_t epochs,
    const T acceptable_deviation,
    const T learn_rate,
    std::vector<std::vector<std::vector<T>>> model) {
  // TODO Assert model

  for (std::size_t n = 0; n < epochs; ++n) {
    for (const auto& sample : samples) {
      std::vector<std::vector<T>> valuess;
      valuess.reserve(model.size() + 1);
      valuess.push_back(sample.first);

      for (const auto& layer : model) {
        std::vector<T> outputs(layer.size(), T(0.0));

        const auto& inputs = valuess.back();
        std::transform(
          layer.cbegin(), layer.cend(),
          outputs.begin(),
          [&inputs](const auto& weights) {
            return std::max(T(0.0), std::inner_product(
              inputs.cbegin(), inputs.cend(),
              weights.cbegin(),
              T(0.0)));
          });
        valuess.push_back(outputs);
      }

      std::vector<std::vector<T>> deviationss;
      deviationss.reserve(model.size());
      deviationss.push_back({sample.second - valuess.back().front()});

      for (auto n = model.size() - 1; n-- > 0;) {
        std::vector<T> deviations(model.at(n).size(), T(0.0));

        const auto& next_layer = model.at(n + 1);
        const auto& next_deviations = deviationss.back();
        for (std::size_t l = 0; l < next_layer.size(); ++l) {
          const auto& next_weights = next_layer.at(l);
          const auto next_deviation = next_deviations.at(l);

          std::transform(
            deviations.cbegin(), deviations.cend(),
            next_weights.cbegin(),
            deviations.begin(),
            [&next_deviation](const auto deviation, const auto next_weight) {
              return deviation + next_weight * next_deviation;
            });
        }

        const auto& values = valuess.at(n + 1);
        std::transform(
          deviations.cbegin(), deviations.cend(),
          values.cbegin(),
          deviations.begin(),
          [](const auto deviation, const auto value) {
            return (value > 0 ? deviation : 0);
          });

        deviationss.push_back(deviations);
      }

      for (std::size_t n = 0; n < model.size(); ++n) {
        auto& layer = model.at(n);

        for (std::size_t k = 0; k < layer.size(); ++k) {
          auto& weights = layer.at(k);
          const auto deviation = deviationss.at(deviationss.size() - n - 1).at(k);

          for (std::size_t l = 0; l < weights.size(); ++l) {
            weights.at(l) += learn_rate * (deviation * valuess.at(n).at(l));
          }
        }
      }
    }
  }

  return model;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("learn", "[helper][learn]") {
  CHECK((mant::learn({{{1.0, 2.0}, 12.0}, {{2.0, 4.0}, 24.0}}, 100, 0.0, 0.0001, {
    {{1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}},
    {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
    {{1.0, 1.0}}
  }) == std::vector<std::vector<std::vector<double>>>({
    {{0.95383787405173315, 0.90767574810346807}, {0.95383787405173315, 0.90767574810346807}, {0.95383787405173315, 0.90767574810346807}},
    {{0.93103069663712112, 0.93103069663712112, 0.93103069663712112}, {0.93103069663712112, 0.93103069663712112, 0.93103069663712112}},
    {{0.77575979608687329, 0.77575979608687329}}
  })));
}
#endif
