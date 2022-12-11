#include <iostream>

#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

#include "fixture.h"

using namespace nlohmann;

class AddRmOperation : public testing::Test
{
 protected:
  JsonLogic* json_logic_;

  void SetUp() override
  {
	json_logic_ = JsonLogic::GetInstance();

	json_logic_->AddOperation("myOperation", [this,x=json_logic_](const json& values, const json& data) {
	  const double a = x->Apply(values.at(0), data); 
	  const double b = x->Apply(values.at(1), data);

	  return a + b;
	});
  }
};

TEST_F(AddRmOperation, AddOperationPrimitive) {
	const auto logic = R"(
		{
			"myOperation": [1, 2]
		}
	)"_json;

  const auto result = json_logic_->Apply(logic);

  EXPECT_EQ(result, 3);
}

TEST_F(AddRmOperation, AddOperationData)
{
  const auto logic = R"(
		{
			"myOperation": [
				{ "var": ["foo.bar"] },
				{ "var": ["foo.baz"] }
			]
		}
	)"_json;

  const auto data = R"(
		{
			"foo": {
				"bar": 1,
				"baz": 2
			}
		}
	)"_json;

  const auto result = json_logic_->Apply(logic, data);

  EXPECT_EQ(result, 3);
}

TEST_F(AddRmOperation, RmOperation)
{
  json_logic_->RmOperation("myOperation");

  const auto logic = R"(
		{
			"myOperation": [1, 2]
		}
	)"_json;

  const auto result = json_logic_->Apply(logic);

  EXPECT_EQ(result, logic);
}
