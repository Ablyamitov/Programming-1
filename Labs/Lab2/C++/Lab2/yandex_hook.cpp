#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace httplib;
json get_webhooks();

enum voice_mode
{
	silent_mode,
	speak_mode
};
enum skill_mode
{
	default_mode,
	help_mode
};
json session_list = json::array();
json default_state_buttons =
{
	{
		{"title", u8"������"},
		{"hide", true}
	},
};
json speak_mode_button =
{
	{"title", u8"��������"},
	{"hide", true}
};
json silent_mode_button =
{
	{"title", u8"�������"},
	{"hide", true}
};

json help_state_buttons =
{
	{
		{"title", u8"�������"},
		{"hide", true}
	},
	{
		{"title", u8"��������"},
		{"hide", true}
	},
	{
		{"title", u8"������"},
		{"hide", true}
	},
	{
		{"title", u8"�������"},
		{"hide", true}
	},
	{
		{"title", u8"����� �� ������"},
		{"hide", true}
	},
	{
		{"title", u8"������� ���������"},
		{"hide", true}
	},
};

json gen_response(const std::string& text,
	const std::string& tts,
	const json& buttons,
	const json* current_session = nullptr,
	const bool end_session = false)
{
	json response = {
		{"response", {
			{"buttons", buttons},
			{"end_session", end_session}
		}},
		{"version", "1.0"}
	};
	if (text != "")
	{
		response["response"]["text"] = text;
	}
	if (current_session != nullptr && (*current_session)["voice_mode"] == speak_mode)
	{
		if (tts != "")
		{
			response["response"]["tts"] = tts;
		}
		response["response"]["buttons"].push_back(silent_mode_button);
	}
	else if (current_session != nullptr && (*current_session)["voice_mode"] == silent_mode)
	{
		response["response"]["buttons"].push_back(speak_mode_button);
	}
	return response;
}

void yandex_hook(const Request& req, Response& res)
{
	json req_json = json::parse(req.body);

	std::string user_id = req_json["session"]["application"]["application_id"];
	json response;
	json* cur_session = nullptr;

	for (auto& session : session_list)
	{
		if (session["user_id"] == user_id)
		{
			cur_session = &session;
			break;
		}
	}

	if (req_json["session"]["new"].get<bool>())
	{
		if (cur_session == nullptr)
		{
			json session =
			{
				{"user_id", user_id},
				{"skill_mode", default_mode},
				{"voice_mode", silent_mode},
				{"cart", json::array()}
			};
			// ������ �����, ��������� � � ������.
			session_list.push_back(session);
			cur_session = &session_list[session_list.size() - 1];
		}
		else
		{
			(*cur_session)["skill_mode"] = default_mode;
			(*cur_session)["voice_mode"] = silent_mode;
		}

		json response = gen_response(
			u8"������������! � ������ ��� � ���������.",
			u8"���+���������! � �����+� ��� � ���+������.",
			default_state_buttons,
			cur_session);

		res.set_content(response.dump(2), "text/json; charset=UTF-8");
		return;
	}

	if (cur_session == nullptr)
	{
		json response = gen_response(
			u8"��������, ��������� ������",
			u8"�����+���, ��������+� ��+����",
			default_state_buttons,
			cur_session,
			true);
		res.set_content(response.dump(2), "text/json; charset=UTF-8");
		return;
	}

	std::string command = req_json["request"]["command"];
	if ((*cur_session)["skill_mode"] == help_mode)
	{
		// �������, ��������, ������, �������, ����� �� ������, ������� ���������, �����
		// � ��� ��� ����������?
		std::string text;
		std::string tts;

		if (command == u8"�������")
		{
			text = u8"������� ��� ��� ����� � � �� ���� ���������� ���� ���������.";
			tts = u8"������� ��� ��� ����� � � �� ���� ���������� ���� ���������.";
		}
		else if (command == u8"��������")
		{
			text = u8"������� ��� �������� � � ���� ���������� ��� ���� �������";
			tts = u8"������� ��� �������� � � ���� ���������� ��� ���� �������";
		}
		else if (command == u8"������")
		{
			text = u8"��� ��, ��� �� ������ ����������. "
				u8"� ���� ������ � �������� ��� �������� � ����� ������������.";
			tts = u8"��� ��, ��� �� ������ ����������. "
				u8"� ���� ������ � �������� ��� �������� � ����� ������������.";
		}
		else if (command == u8"�������")
		{
			text = u8"� �������, ��� �� ������ ��������� ��� ������� �� �������.\n"
				u8"����� �������� ���-��, ������� \"�������� � ������� ��� 5 ������\".\n"
				u8"����� ������� ���-��, ������� \"������� �� ������� ���\".";
			tts = u8"� �������, ��� �� ������ ��������� ��� ������� �� �������.\n"
				u8"����� �������� ���-��, ������� \"�������� � ������� ��� 5 ������\".\n"
				u8"����� ������� ���-��, ������� \"������� �� ������� ���\".";
		}
		else if (command == u8"�����")
		{
			text = u8"� ��������� ��������� ���� ������� � ����� ������� � ������ �� ����� ���������.";
			tts = u8"� ��������� ��������� ���� ������� � ����� ������� � ������ �� ����� ���������.";
		}
		else if (command == u8"������� ���������")
		{
			text = u8"� ������� ������ ����� ������� ���-�����, ����� ���� ����� �������.";
			tts = u8"� ������� ������ ����� ������� ���-�����, ����� ���� ����� �������.";
		}
		else if (command == u8"����� �� ������")
		{
			text = u8"������. ����� ����� ������ - �����������.";
			tts = u8"������. ����� ����� ������ - �����������.";
			(*cur_session)["skill_mode"] = default_mode;
		}
		else
		{
			text = u8"� ���� � �� ���� ��� ������.";
			tts = u8"� +���� � �� ���+� ��� ���+���.";
		}

		json response;
		if ((*cur_session)["skill_mode"] == help_mode)
		{
			response = gen_response(text, tts, help_state_buttons, cur_session);
		}
		else
		{
			response = gen_response(text, tts, default_state_buttons, cur_session);
		}
		res.set_content(response.dump(2), "text/json; charset=UTF-8");
	}
	else
	{
		if (command == u8"�������")
		{
			std::string text = u8"�����, �����";
			std::string tts;
			(*cur_session)["voice_mode"] = silent_mode;
			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session);
			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else if (command == u8"��������")
		{
			std::string text = u8"������.";
			std::string tts = u8"�����+�.";
			(*cur_session)["voice_mode"] = speak_mode;
			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session);
			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else if (command == u8"������")
		{
			std::string text =
				u8"�������� ��� �������. �������� � ��������� ��������� �����.\n"
				u8"�������. ��������� ����� ������ �������, � ��� �� �� ���������.\n"
				u8"������. ������������ � ������������ ����� ������.\n"
				u8"��������� �������. ������� ������� � ��������� ������ � ������� ������.\n"
				u8"�����. ������� ����� ������� � �������� � ���.\n"
				u8"� ��� ���������� ���������?";
			std::string tts =
				u8"�������� ��� �������. �������� � ��������� ��������� �����.\n"
				u8"�������. ��������� ����� ������ �������, � ��� �� �� ���������.\n"
				u8"������. ������������ � ������������ ����� ������.\n"
				u8"��������� �������. ������� ������� � ��������� ������ � ������� ������.\n"
				u8"�����. ������� ����� ������� � �������� � ���.\n"
				u8"� ��� ���������� ���������?";
			json response = gen_response(
				text,
				tts,
				help_state_buttons,
				cur_session);
			(*cur_session)["skill_mode"] = help_mode;
			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else if (command == u8"�������� �������")
		{
			std::string text = u8"������� �����.";
			std::string tts = u8"�����+��� ����+�.";
			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session);
			(*cur_session).erase("cart");
			(*cur_session)["cart"] = json::array();
			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else if (command == u8"��� � �������")
		{
			std::cout << "cart: " << (*cur_session)["cart"] << std::endl;

			std::string text;
			std::string tts;

			if ((*cur_session)["cart"].empty())
			{
				text = u8"�� ������ ������ ���� ������� �����.";
				tts = u8"�� ������ ������ ���� ������� �����.";
			}
			else
			{
				text = u8"�� ������ ������ � ����� �������:";
				for (auto& elem : (*cur_session)["cart"])
				{
					int price = elem["price"].get<int>();

					text += "\n"
						+ elem["item"].get<std::string>()
						+ u8" ����� "
						+ std::to_string(price);

					if (price % 10 == 0)
					{
						text += u8" ������,";
					}
					else if (price % 10 == 1)
					{
						text += u8" �����,";
					}
					else if (price % 10 < 5 && price % 10 > 0)
					{
						text += u8" �����,";
					}
					else
					{
						text += u8" ������,";
					}
				}
				text.pop_back();
				tts = text;
			}

			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session);
			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else if (command == u8"������� ���������")
		{
			std::string text = u8"�������� ���!";
			std::string tts = u8"�����+��� ��+�!";

			json output =
			{
				{"user_id", user_id},
				{"cart", (*cur_session)["cart"]}
			};

			json config = get_webhooks();

			for (std::string link : config["webhooks"])
			{
				std::cout << "link " << link << std::endl;
				int index = link.find('/', static_cast<std::string>("https://").size());

				std::cout << (index == std::string::npos) << std::endl;
				std::cout << (link.find("http://") == 0) << std::endl;
				std::cout << (link.find("https://") == 0) << std::endl;

				if (index != -1)
				{
					std::cout << link.substr(0, index).c_str() << std::endl;
					std::cout << link.substr(index, link.size()).c_str() << std::endl;
				}

				if (index == std::string::npos) // || link.find("http://") == 0 || link.find("https://") == 0)
				{
					std::cout << "bad link " << link << std::endl;
				}
				else
				{
					std::cout << link.substr(0, index).c_str() << std::endl;
					std::cout << link.substr(index, link.size()).c_str() << std::endl;
					Client cli(link.substr(0, index).c_str());
					cli.Post(link.substr(index, -1).c_str(), output.dump(2), "application/json; charset=UTF-8");
				}
			}

			(*cur_session).erase("cart");
			(*cur_session)["cart"] = json::array();

			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session,
				true);
			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else if (command == u8"�����")
		{
			std::string text = "";
			std::string tts = "";

			size_t size = req_json["request"]["nlu"]["tokens"].size();
			int sum = 0;
			for (auto& cart_item : (*cur_session)["cart"])
			{
				sum += cart_item["price"].get<int>();
			}
			if (sum == 0)
			{
				text = u8"� ��� � ������� ��� �������.";
				tts = u8"� ��� � ����+��� ��� ���+����.";
			}
			else
			{
				text = u8"� ����� ����� � ��� ������� �� " + std::to_string(sum);
				tts = u8"� +����� �+���� � ��� ���+���� �� " + std::to_string(sum);
				if (sum % 10 == 0)
				{
					text += u8" ������.";
					tts += u8" ����+��.";
				}
				else if (sum % 10 == 1)
				{
					text += u8" �����.";
					tts += u8" �����.";
				}
				else if (sum % 10 < 5 && sum % 10 > 0)
				{
					text += u8" �����.";
					tts += u8" ����+�.";
				}
				else
				{
					text += u8" ������.";
					tts += u8" ����+��.";
				}
			}

			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session);
			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else if (command.find(u8"�������� � �������") == 0 || command.find(u8"������ � �������") == 0)
		{
			size_t size = req_json["request"]["nlu"]["tokens"].size();
			std::string text = u8"��.";
			std::string tts = u8"��+��.";
			std::string item_name;
			int			item_price = 0;
			int			number_index = 0;
			bool			number_index_set = false;

			for (auto entity : req_json["request"]["nlu"]["entities"])
			{
				if (entity["type"].get<std::string>() == "YANDEX.NUMBER")
				{
					number_index = entity["tokens"]["start"];
					std::cout << entity["value"].type_name() << std::endl;
					int val = entity["value"];
					std::cout << u8"����: ���� " << val << std::endl;
					if (val < 0)
					{
						text = u8"���� �� ����� ���� �������������.";
						tts = u8"���+� �� �+���� �+��� �����+��������.";
					}
					else if (val == 0)
					{
						text = u8"�� ��� �� ��� ������� ���-�� �� ���������?";
						tts = u8"�� ��� �� ��� ����+��� ��+� �� �� �����+����?";
					}
					else
					{
						item_price = val;
					}
					number_index_set = true;
					break;
				}
			}
			if (size == 3)
			{
				text = u8"����������, ����������, ��� �������� � �������.";
				tts = u8"���+�������, �������+���, ��� ���+����� � ����+���.";
			}
			else if (!number_index_set)
			{
				text = u8"����������, ������� ���� ������.";
				tts = u8"���+�������, ����+��� �+��� ���+���.";
			}
			else if (number_index == 3)
			{
				text = u8"����������, ������� �������� ������.";
				tts = u8"���+�������, ����+��� ����+���� ���+���.";
			}
			else
			{
				for (int i = 3; i < number_index; ++i)
				{
					item_name += req_json["request"]["nlu"]["tokens"][i].get<std::string>();
					item_name += " ";
				}
				item_name.pop_back();
				json item = {
					{"item",  item_name},
					{"price", item_price}
				};
				(*cur_session)["cart"].push_back(item);
			}

			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session);

			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else if (command.find(u8"������� �� �������") == 0 || command.find(u8"����� �� �������") == 0
			|| command.find(u8"������ �� �������") == 0 || command.find(u8"����� �� �������") == 0)
		{
			std::cout << (*cur_session)["cart"] << std::endl;
			size_t size = req_json["request"]["nlu"]["tokens"].size();

			std::string text;
			std::string tts;
			std::string item_name = "";

			for (int i = 3; i < size; ++i)
			{
				std::cout << req_json["request"]["nlu"]["tokens"][i].get<std::string>() << std::endl;
				item_name += req_json["request"]["nlu"]["tokens"][i].get<std::string>();
				item_name += " ";
			}
			bool found_item = false;
			int	item_index = 0;

			if (item_name == "")
			{
				text = u8"�� �� ���� �������, ��� ����������� �������.";
				tts = u8"�� �� ���� ����+���, ��� �����+������ ����+���.";
			}
			else
			{
				item_name.pop_back();
				for (auto& cart_item : (*cur_session)["cart"])
				{
					if (cart_item["item"].get<std::string>() == item_name)
					{
						found_item = true;
						break;
					}
					++item_index;
				}
				if (!found_item)
				{
					std::cout << u8"����: ����� ������� �� ��� ������" << std::endl;
					text = u8"������ � ����� ������� ����";
					tts = u8"���+��� � �+���� ����+��� �+���.";
				}
				else
				{
					std::cout << u8"����: ����� ����� �������" << std::endl;
					text = u8"�������.";
					tts = u8"����+���";
					(*cur_session)["cart"].erase((*cur_session)["cart"].begin() + item_index);
					std::cout << u8"����: ������� �������" << std::endl;
				}
			}
			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session);
			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
		else
		{
			std::string text = u8"� �� ���� ����� �������.";
			std::string tts = u8"� �� ��+�� ���+�� ���+����.";

			json response = gen_response(
				text,
				tts,
				default_state_buttons,
				cur_session);

			res.set_content(response.dump(2), "text/json; charset=UTF-8");
		}
	}
	std::cout << std::endl;
}