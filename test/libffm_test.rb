require_relative "test_helper"

class LibffmTest < Minitest::Test
  def test_works
    model = Libffm::Model.new
    model.fit("test/support/data.txt")

    predictions = model.predict("test/support/data.txt")
    assert_in_delta 0.474183, predictions[0]
    assert_in_delta 0.660218, predictions[1]

    path = File.join(Dir.mktmpdir, "model.bin")
    model.save_model(path)
    assert File.exist?(path)

    model = Libffm::Model.new
    model.load_model(path)

    predictions = model.predict("test/support/data.txt")
    assert_in_delta 0.474183, predictions[0]
    assert_in_delta 0.660218, predictions[1]
  end

  def test_eval_set
    model = Libffm::Model.new
    model.fit("test/support/data.txt", eval_set: "test/support/data.txt")
  end

  def test_not_fit
    model = Libffm::Model.new
    error = assert_raises do
      model.predict("data.txt")
    end
    assert_equal "Not fit", error.message
  end
end
