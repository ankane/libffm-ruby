#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <ffm.h>
#include <rice/rice.hpp>

extern "C"
void Init_ext() {
  auto rb_mLibffm = Rice::define_module("Libffm");

  auto rb_mExt = Rice::define_module_under(rb_mLibffm, "Ext");
  Rice::define_class_under<ffm::ffm_model>(rb_mExt, "Model");

  rb_mExt
    .define_singleton_function(
      "release_model",
      [](ffm::ffm_model& model) {
        model.release();
      })
    .define_singleton_function(
      "fit",
      [](Rice::String tr_path, Rice::String va_path, Rice::String tmp_prefix, ffm::ffm_float eta, ffm::ffm_float lambda, ffm::ffm_int nr_iters, ffm::ffm_int k, bool normalization, bool auto_stop) {
        // quiet
        std::cout.setstate(std::ios_base::badbit);

        std::string tr_bin_path = tmp_prefix.str() + "train.bin";
        ffm::ffm_read_problem_to_disk(tr_path.str(), tr_bin_path);

        std::string va_bin_path;
        if (va_path.length() != 0) {
          va_bin_path = tmp_prefix.str() + "validation.bin";
          ffm::ffm_read_problem_to_disk(va_path.str(), va_bin_path);
        }

        ffm::ffm_parameter param;
        param.eta = eta;
        param.lambda = lambda;
        param.nr_iters = nr_iters;
        param.k = k;
        param.normalization = normalization;
        param.auto_stop = auto_stop;

        return ffm::ffm_train_on_disk(tr_bin_path, va_bin_path, param);
      })
    .define_singleton_function(
      "predict",
      [](ffm::ffm_model& model, Rice::String test_path) {
        std::ifstream f_in(test_path.str());
        if (!f_in.is_open()) {
          throw std::runtime_error{"Cannot open file"};
        }
        std::string line;

        Rice::Array ret;
        while (std::getline(f_in, line)) {
          std::vector<ffm::ffm_node> x;

          std::string_view line_view{line};

          size_t n = line_view.find_first_of(" \t");
          if (n == std::string_view::npos) {
            throw std::runtime_error{"Invalid line"};
          }

          size_t start = n + 1;

          while (true) {
            n = line_view.find_first_of(" \t", start);

            std::string_view s = n == std::string_view::npos ? line_view.substr(start) : line_view.substr(start, n - start);

            size_t n2 = s.find(':');
            if (n2 == std::string_view::npos) {
              throw std::runtime_error{"Invalid line"};
            }

            size_t n3 = s.find(':', n2 + 1);
            if (n3 == std::string_view::npos) {
              throw std::runtime_error{"Invalid line"};
            }

            std::string_view field_char = s.substr(0, n2);
            std::string_view idx_char = s.substr(n2 + 1, n3 - n2 - 1);
            std::string_view value_char = s.substr(n3 + 1);

            // cannot pass string view to stoi/stof
            ffm::ffm_node N{
              std::stoi(std::string{field_char}),
              std::stoi(std::string{idx_char}),
              std::stof(std::string{value_char})
            };

            x.push_back(N);

            if (n == std::string::npos) {
              break;
            }

            start = n + 1;
          }

          ffm::ffm_float y_bar = ffm::ffm_predict(x.data(), x.data() + x.size(), model);
          ret.push(y_bar, false);
        }

        return ret;
      })
    .define_singleton_function(
      "save_model",
      [](ffm::ffm_model& model, Rice::String path) {
        ffm::ffm_save_model(model, path.str());
      })
    .define_singleton_function(
      "load_model",
      [](Rice::String path) {
        return ffm::ffm_load_model(path.str());
      });
}
