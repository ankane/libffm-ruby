module Libffm
  class Model
    def initialize(eta: 0.2, lambda: 0.00002, nr_iters: 15, k: 4, normalization: true, auto_stop: false)
      @eta = eta
      @lambda = lambda
      @nr_iters = nr_iters
      @k = k
      @normalization = normalization
      @auto_stop = auto_stop

      @model = nil
    end

    def fit(data, eval_set: nil)
      Dir.mktmpdir do |dir|
        @model = Ext.fit(data, eval_set || "", File.join(dir, ""), @eta, @lambda, @nr_iters, @k, @normalization, @auto_stop)
        add_finalizer(@model)
      end
    end

    def predict(data)
      raise "Not fit" unless @model
      Ext.predict(@model, data)
    end

    def save_model(path)
      Ext.save_model(@model, path)
    end

    def load_model(path)
      @model = Ext.load_model(path)
      add_finalizer(@model)
    end

    private

    def add_finalizer(model)
      ObjectSpace.define_finalizer(model, self.class.finalize_model(model))
    end

    def self.finalize_model(model)
      proc { Ext.finalize_model(model) }
    end
  end
end
