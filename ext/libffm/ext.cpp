#include <cstdio>
#include <iostream>
#include <string>

#include <ffm.h>
#include <rice/rice.hpp>
#include <rice/stl.hpp>

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
      [](const std::string& tr_path, const std::string& va_path, const std::string& tmp_prefix, ffm::ffm_float eta, ffm::ffm_float lambda, ffm::ffm_int nr_iters, ffm::ffm_int k, bool normalization, bool auto_stop) {
        // quiet
        ffm::cout.setstate(ffm::ios_base::badbit);

        std::string tr_bin_path = tmp_prefix + "train.bin";
        ffm::ffm_read_problem_to_disk(tr_path, tr_bin_path);

        std::string va_bin_path = "";
        if (va_path.size() > 0) {
          va_bin_path = tmp_prefix + "validation.bin";
          ffm::ffm_read_problem_to_disk(va_path, va_bin_path);
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
      [](ffm::ffm_model& model, const std::string& test_path) {
        int const kMaxLineSize = 1000000;

        FILE *f_in = fopen(test_path.c_str(), "r");
        char line[kMaxLineSize];

        ffm::vector<ffm::ffm_node> x;
        ffm::ffm_int i = 0;

        Rice::Array ret;
        for(; fgets(line, kMaxLineSize, f_in) != nullptr; i++) {
          x.clear();
          strtok(line, " \t");

          while (true) {
            char *field_char = strtok(nullptr, ":");
            char *idx_char = strtok(nullptr, ":");
            char *value_char = strtok(nullptr, " \t");
            if (field_char == nullptr || *field_char == '\n')
                break;

            ffm::ffm_node N;
            N.f = atoi(field_char);
            N.j = atoi(idx_char);
            N.v = atof(value_char);

            x.push_back(N);
          }

          ffm::ffm_float y_bar = ffm::ffm_predict(x.data(), x.data()+x.size(), model);
          ret.push(y_bar);
        }

        fclose(f_in);

        return ret;
      })
    .define_singleton_function(
      "save_model",
      [](ffm::ffm_model& model, const std::string& path) {
        ffm::ffm_save_model(model, path);
      })
    .define_singleton_function(
      "load_model",
      [](const std::string& path) {
        return ffm::ffm_load_model(path);
      });
}
