# LIBFFM Ruby

[LIBFFM](https://github.com/ycjuan/libffm) - field-aware factorization machines - for Ruby

[![Build Status](https://github.com/ankane/libffm-ruby/actions/workflows/build.yml/badge.svg)](https://github.com/ankane/libffm-ruby/actions)

## Installation

Add this line to your application’s Gemfile:

```ruby
gem "libffm"
```

## Getting Started

Prep your data in LIBFFM format

```txt
0 0:0:1 1:1:1
1 0:2:1 1:3:1
```

Create a model

```ruby
model = Libffm::Model.new
model.fit("train.txt")
```

Make predictions

```ruby
model.predict("test.txt")
```

Save the model to a file

```ruby
model.save_model("model.bin")
```

Load the model from a file

```ruby
model.load_model("model.bin")
```

Pass a validation set

```ruby
model.fit("train.txt", eval_set: "validation.txt")
```

## Parameters

Pass parameters - default values below

```ruby
Libffm::Model.new(
  eta: 0.2,                # learning rate
  lambda: 0.00002,         # regularization parameter
  nr_iters: 15,            # number of iterations
  k: 4,                    # number of latent factors
  normalization: true,     # use instance-wise normalization
  auto_stop: false         # stop at the iteration that achieves the best validation loss
)
```

## History

View the [changelog](https://github.com/ankane/libffm-ruby/blob/master/CHANGELOG.md)

## Contributing

Everyone is encouraged to help improve this project. Here are a few ways you can help:

- [Report bugs](https://github.com/ankane/libffm-ruby/issues)
- Fix bugs and [submit pull requests](https://github.com/ankane/libffm-ruby/pulls)
- Write, clarify, or fix documentation
- Suggest or add new features

To get started with development:

```sh
git clone --recursive https://github.com/ankane/libffm-ruby.git
cd libffm-ruby
bundle install
bundle exec rake compile
bundle exec rake test
```
