#pragma once
//
//#include <vector>
//#include <memory>
//#include <sstream>
//#include <algorithm>
//#include <iterator>
//#include <cstdlib>
//#include <thread>
//#include <deque>
//#include <mutex>
//#include <istream>
//#include <Windows.h>
//
//namespace core::util {
//	class sigscan_params {
//	public:
//		sigscan_params() : m_signature{}, m_found{} {
//			m_offset = m_dereferences = 0;
//		}
//
//		std::vector<uint8_t> m_signature;
//		uintptr_t m_start;
//		size_t m_length;
//		void **m_output;
//		bool m_found;
//
//		int m_offset;
//		int m_dereferences;
//	};
//
//	class queued_sigscanner {
//	protected:
//		std::mutex m_signatures_mutex;
//
//		void tokenize_signature(const std::string &signature, std::vector<std::string> &out) {
//			using namespace std;
//
//			out = {
//				istream_iterator<string>{istringstream(signature) >> skipws},
//				istream_iterator<string>{}
//			};
//		}
//
//		sigscan_params &construct_signature(const std::string &sig) {
//			using namespace std;
//
//			auto &scan_params{m_signatures.emplace_back(sigscan_params())};
//			vector<string> tokens;
//
//			tokenize_signature(sig, tokens);
//
//			for (auto &token : tokens) {
//				std::string clean_token(token);
//				clean_token.erase(std::remove(clean_token.begin(), clean_token.end(), '*'), clean_token.end());
//				clean_token.erase(std::remove(clean_token.begin(), clean_token.end(), '.'), clean_token.end());
//
//				if (clean_token.empty())
//					continue;
//
//				scan_params.m_signature.push_back((uint8_t) std::strtoul(clean_token.c_str(), nullptr, 16));
//
//				if ((token.front() == '.' && token.back() != '.')) {
//					scan_params.m_offset = (scan_params.m_signature.size() - 1);
//				} else if ((token.front() == '*' && token.back() != '*')) {
//					for (scan_params.m_dereferences = 0;
//						 scan_params.m_dereferences < (int) token.size();
//						 ++scan_params.m_dereferences) {
//						if (token.at(scan_params.m_dereferences) != '*')
//							break;
//					}
//
//					scan_params.m_offset = (scan_params.m_signature.size() - 1);
//				}
//			}
//
//			return scan_params;
//		}
//
//	public:
//		void worker_thread() {
//			sigscan_params params;
//			uint8_t *start;
//			printf("running this stuffs.");
//			for (;;) {
//				m_signatures_mutex.lock();
//				{
//					if (m_signatures.empty()) {
//						m_signatures_mutex.unlock();
//						return;
//					}
//printf("11111111");
//					params = m_signatures.front();
//					m_signatures.pop_front();
//				}
//				m_signatures_mutex.unlock();
//
//				start = (uint8_t *) params.m_start;
//
//				if (params.m_signature.empty())
//					continue;
//printf("nocont.");
//				for (size_t i{}; i < params.m_length; ++i) {
//					if (start[i] != params.m_signature.at(0))
//						continue;
//
//					auto cmp{start + i};
//					bool found{true};
//
//					for (auto &b : params.m_signature) {
//						if (b && (b != *cmp)) {
//							found = false;
//							break;
//						}
//printf("params.");
//						++cmp;
//					}
//
//					if (found) {
//						start += (i + params.m_offset);
//
//						for (int i{}; i < params.m_dereferences; ++i)
//							start = *(uint8_t **) start;
//printf("break");
//						*params.m_output = start;
//						break;
//					}
//				}
//			}
//		}
//
//		queued_sigscanner() = default;
//
//		template<typename T1, typename T2>
//		sigscan_params  &add_signature(T1 start, T2 &out, const std::string &signature, size_t length) {
//			auto &scan_params{construct_signature(signature)};
//
//			scan_params.m_length = length;
//			scan_params.m_start = uintptr_t(start);
//			scan_params.m_output = (void **) out;
//
//			return scan_params;
//		}
//
//		template<typename T1>
//		auto &add_signature(IMAGE_DOS_HEADER *mod, T1 &out, const std::string &signature, size_t length = 0) {
//			auto nt_headers{(IMAGE_NT_HEADERS *) (uintptr_t(mod) + mod->e_lfanew)};
//
//			return add_signature(
//				uintptr_t(mod) + nt_headers->OptionalHeader.BaseOfCode,
//				out,
//				signature,
//				(!!length ? length : nt_headers->OptionalHeader.SizeOfCode)
//			);
//		}
//
//		template<typename T1>
//		auto &add_signature(HMODULE mod, T1 &out, const std::string &signature, size_t length = 0) {
//			return add_signature<T1>((IMAGE_DOS_HEADER *) mod, out, signature, length);
//		}
//
//		template<typename T>
//		auto &addsignature(T &out, const std::string &signature) {
//			return add_signature(GetModuleHandleA("haloce.exe"), out, signature);
//		}
//
//		void scan() {
//			std::vector<std::thread> threads;
//
//			for (std::size_t i{}; i < std::thread::hardware_concurrency(); ++i)
//				threads.emplace_back(std::thread([this]() { this->worker_thread(); }));
//
//			for (auto &&thread : threads)
//				thread.join();
//		}
//
//	protected:
//		std::deque<sigscan_params> m_signatures;
//	};
//};
