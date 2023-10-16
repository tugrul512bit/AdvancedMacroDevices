#pragma once
#include "AppStructure.h"
#include<functional>
#include<vector>
#include<string>
#include<opencv2/opencv.hpp>
#include <GL/glcorearb.h>
#include "Paint.h"
#include <map>
#include <mutex>
namespace Window
{

	struct TextureStorage
	{
		std::map<std::string, GLuint> strToTex;
		GLuint Generate(std::string name, cv::Mat img)
		{

			if (strToTex.find(name) == strToTex.end())
			{
				GLuint textureID;
				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				GLenum inputColourFormat = GL_RGBA;


				glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RGBA,
					img.cols,
					img.rows,
					0,
					inputColourFormat,
					GL_UNSIGNED_BYTE,
					img.data);

				strToTex[name] = textureID;
				return textureID;
			}
			else
				return strToTex[name];
		}
		~TextureStorage()
		{
			for (auto e : strToTex)
			{
				glDeleteTextures(1, &e.second);
			}
		}
	};
	static TextureStorage globalTextures;

	class ImageItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, bool sameLine, cv::Mat img, int busynessLevel, bool deadlock, int parallelism)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<ImageItem>(new ImageItem(name, sameLine, img, busynessLevel, deadlock, parallelism), [](ImageItem* i) { delete i; });
			return node;
		}

		ImageItem(std::string name = "text", bool sameLine = false, cv::Mat img=cv::Mat(64,64,CV_8UC4), int busynessLevel = 0,bool deadlock=false, int parallelism=0)
		{
			_name = name;
			_sameLine = sameLine;
			_width = img.cols;
			_height = img.rows;
			_img = cv::Mat(_height,_width,CV_8UC4);

			if(img.dataend - img.data == _height*_width)
				for (int i = 0; i < _height * _width; i++)
				{
					_img.at<unsigned char>(i*4) = img.at<unsigned char>(i);
					_img.at<unsigned char>(i * 4+1) = img.at<unsigned char>(i);
					_img.at<unsigned char>(i * 4+2) = img.at<unsigned char>(i);
					_img.at<unsigned char>(i * 4+3) = 255;
				}
			else if (img.dataend - img.data == _height * _width * 2)
				for (int i = 0; i < _height * _width; i++)
				{
					_img.at<unsigned char>(i * 4) = img.at<unsigned char>(i*2);
					_img.at<unsigned char>(i * 4 + 1) = img.at<unsigned char>(i*2+1);
					_img.at<unsigned char>(i * 4 + 2) = 255;
					_img.at<unsigned char>(i * 4+3) = 255;
				}
			else if (img.dataend - img.data == _height * _width * 3)
				for (int i = 0; i < _height * _width; i++)
				{
					_img.at<unsigned char>(i * 4) = img.at<unsigned char>(i * 3);
					_img.at<unsigned char>(i * 4 + 1) = img.at<unsigned char>(i * 3 + 1);
					_img.at<unsigned char>(i * 4 + 2) = img.at<unsigned char>(i * 3 + 2);
					_img.at<unsigned char>(i * 4+3) = 255;
				}
			else if (img.dataend - img.data == _height * _width * 4)
				for (int i = 0; i < _height * _width; i++)
				{
					_img.at<unsigned char>(i * 4) = img.at<unsigned char>(i * 4);
					_img.at<unsigned char>(i * 4 + 1) = img.at<unsigned char>(i * 4 + 1);
					_img.at<unsigned char>(i * 4 + 2) = img.at<unsigned char>(i * 4 + 2);
					_img.at<unsigned char>(i * 4+3) = img.at<unsigned char>(i * 4 + 3);
				}
			if (busynessLevel>0)
			{
				
				for (int i = 0; i < _img.rows * _img.cols; i++)
					_img.at<cv::Vec4b>(i) = cv::Vec4b
					(
						0,
						_img.at<cv::Vec4b>(i).val[1] * busynessLevel / 100.0,
						0,
						_img.at<cv::Vec4b>(i).val[3]
					);

				if (busynessLevel >= 95)
				{
					cv::rectangle(_img, cv::Rect2f(52, 55, 5, 5), cv::Scalar(0, 255 * busynessLevel / 100.0, 0, 255), -1);
				}
				if (busynessLevel >= 70)
				{
					cv::rectangle(_img, cv::Rect2f(37, 55, 5, 5), cv::Scalar(0, 255 * busynessLevel / 100.0, 0, 255), -1);
				}
				if (busynessLevel >= 45)
				{
					cv::rectangle(_img, cv::Rect2f(22, 55, 5, 5), cv::Scalar(0, 255 * busynessLevel / 100.0, 0, 255), -1);
				}
				if (busynessLevel >= 20)
				{
					cv::rectangle(_img, cv::Rect2f(7, 55, 5, 5), cv::Scalar(0, 255 * busynessLevel / 100.0, 0, 255), -1);
				}
			}
			if (deadlock)
			{
				for (int i = 0; i < _img.rows * _img.cols; i++)
					_img.at<cv::Vec4b>(i) = cv::Vec4b
					(
						_img.at<cv::Vec4b>(i).val[1],
						0,
						0,
						_img.at<cv::Vec4b>(i).val[3]
					);
			}

			if (parallelism > 0)
			{
				auto num = std::to_string(parallelism);
				cv::putText(_img, cv::String(num), cv::Point2f(50, 18), 1, 1.2, cv::Scalar(44, 0, 255, 255),2);
			}

			_deadlock = deadlock;
			_busynessLevel = busynessLevel;
			_vec1 = ImVec2(_width, _height);
			_parallelism = parallelism;
			_texture = ToTexture();

		}

		void Compute() override
		{

		}

		void PreRender() override
		{			
			ImGui::Image((void*)(intptr_t)_texture, _vec1);
		}

		void PostRender() override
		{
		}

	private:
		int _width;
		int _height;
		GLuint  _texture;
		cv::Mat _img;
		ImVec2 _vec1;
		int _busynessLevel;
		bool _deadlock;
		int _parallelism;
        GLuint ToTexture() {       
            return globalTextures.Generate(_name+std::string("_")+std::to_string(_busynessLevel)+std::string("_")+std::to_string(_deadlock)+std::string("_")+std::to_string(_parallelism), _img);
        }
	};

	namespace Images
	{
		cv::Mat AluImage()
		{
			cv::Mat img(64, 64, CV_8UC4);
			img = 0;
			// border
			cv::line(img, cv::Point2f(0, 0), cv::Point2f(63, 0), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 0), cv::Point2f(63, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 63), cv::Point2f(0, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(0, 63), cv::Point2f(0, 0), cv::Scalar(255, 255, 255, 255), 1);

			// content
			int yOffset = 20;
			cv::line(img, cv::Point2f(5, 5+ yOffset), cv::Point2f(20, 5+ yOffset), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(44, 5+ yOffset), cv::Point2f(59, 5+ yOffset), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(20, 5+ yOffset), cv::Point2f(32, 15+ yOffset), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(32, 15+ yOffset), cv::Point2f(44, 5+ yOffset), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(5, 5+ yOffset), cv::Point2f(22, 25+ yOffset), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(22, 25+ yOffset), cv::Point2f(42, 25+ yOffset), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(42, 25+ yOffset), cv::Point2f(59, 5+ yOffset), cv::Scalar(255, 255, 255, 255), 2);			
			img = Global::ImageProcessing::FloodFill(32, 37, img, 255, 255, 255, 255);
			cv::putText(img, cv::String("+-/*"), cv::Point2f(15, 21 + yOffset), 1, 0.6, cv::Scalar(0, 0, 0, 255));
			cv::putText(img, cv::String("ALU"), cv::Point2f(19, 22), 1, 0.9, cv::Scalar(255, 255, 255, 255));			
			return img;
		}

		cv::Mat FpuImage()
		{
			cv::Mat img(64, 64, CV_8UC4);
			img = 0;
			// border
			cv::line(img, cv::Point2f(0, 0), cv::Point2f(63, 0), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 0), cv::Point2f(63, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 63), cv::Point2f(0, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(0, 63), cv::Point2f(0, 0), cv::Scalar(255, 255, 255, 255), 1);

			// content
			
			cv::line(img, cv::Point2f(5, 20 ), cv::Point2f(40, 20), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(5, 40), cv::Point2f(40, 40), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(5, 20), cv::Point2f(5, 40), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(40, 20), cv::Point2f(40, 40), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(25, 43), cv::Point2f(25, 57), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(25, 57), cv::Point2f(48, 57), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(48, 57), cv::Point2f(48, 5), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(48, 5), cv::Point2f(28, 5), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(28, 5), cv::Point2f(28, 15), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(28, 15), cv::Point2f(23, 10), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(28, 15), cv::Point2f(33, 10), cv::Scalar(255, 255, 255, 255), 1);

			cv::putText(img, cv::String("sqrt"), cv::Point2f(7, 35), 1, 0.9, cv::Scalar(255, 255, 255, 255));
			return img;
		}

		cv::Mat CacheImage()
		{
			cv::Mat img(64, 64, CV_8UC4);
			img = 0;
			// border
			cv::line(img, cv::Point2f(0, 0), cv::Point2f(63, 0), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 0), cv::Point2f(63, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 63), cv::Point2f(0, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(0, 63), cv::Point2f(0, 0), cv::Scalar(255, 255, 255, 255), 1);

			// content
			
			for (int i = 0; i < 5; i++)
			{

				cv::line(img, cv::Point2f(5, 5 + i*12), cv::Point2f(59, 5 + i * 12), cv::Scalar(255, 255, 255, 255), 2);
				cv::line(img, cv::Point2f(5, 10 + i * 12), cv::Point2f(59, 10 + i * 12), cv::Scalar(255, 255, 255, 255), 2);
				cv::line(img, cv::Point2f(5, 5 + i * 12), cv::Point2f(5, 10 + i * 12), cv::Scalar(255, 255, 255, 255), 2);
				cv::line(img, cv::Point2f(59, 10 + i * 12), cv::Point2f(59, 5 + i * 12), cv::Scalar(255, 255, 255, 255), 2);
				img = Global::ImageProcessing::FloodFill(8, 8 + i * 12, img, 255, 255, 255, 255);
				cv::putText(img, cv::String("cache line"), cv::Point2f(5, 11 + i * 12), 1, 0.6, cv::Scalar(0, 0, 0, 255));
			}
			return img;
		}

		cv::Mat RegisterImage()
		{
			cv::Mat img(64, 64, CV_8UC4);
			img = 0;
			// border
			cv::line(img, cv::Point2f(0, 0), cv::Point2f(63, 0), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 0), cv::Point2f(63, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 63), cv::Point2f(0, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(0, 63), cv::Point2f(0, 0), cv::Scalar(255, 255, 255, 255), 1);

			// content
			for (int j = 0; j < 5; j++)
			for (int i = 0; i < 5; i++)
			{
				cv::line(img, cv::Point2f(4 + j*12, 4 + i * 12), cv::Point2f(11 + j * 12, 4 + i * 12), cv::Scalar(255, 255, 255, 255), 1);
				cv::line(img, cv::Point2f(11 + j * 12, 4 + i * 12), cv::Point2f(11 + j* 12, 11 + i * 12), cv::Scalar(255, 255, 255, 255), 1);
				cv::line(img, cv::Point2f(11 + j * 12, 11 + i * 12), cv::Point2f(4 + j * 12, 11 + i * 12), cv::Scalar(255, 255, 255, 255), 1);
				cv::line(img, cv::Point2f(4 + j * 12, 11 + i * 12), cv::Point2f(4 + j * 12, 4 + i * 12), cv::Scalar(255, 255, 255, 255), 1);

				img = Global::ImageProcessing::FloodFill(8+j* 12, 8 + i * 12, img, 255, 255, 255, 255);
				cv::putText(img, cv::String("R"), cv::Point2f(5+j* 12, 10 + i * 12), 1, 0.4, cv::Scalar(0, 0, 0, 255));
			}
			return img;
		}

		cv::Mat BusImage()
		{
			cv::Mat img(64, 64, CV_8UC4);
			img = 0;
			
			// content			
			cv::line(img, cv::Point2f(28, 0), cv::Point2f(28, 28), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(36, 0), cv::Point2f(36, 28), cv::Scalar(255, 255, 255, 255), 1);

			cv::line(img, cv::Point2f(28, 63), cv::Point2f(28, 35), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(36, 63), cv::Point2f(36, 35), cv::Scalar(255, 255, 255, 255), 1);

			cv::line(img, cv::Point2f(0, 28), cv::Point2f(28, 28), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(0, 35), cv::Point2f(28, 35), cv::Scalar(255, 255, 255, 255), 1);

			cv::line(img, cv::Point2f(36, 28), cv::Point2f(63, 28), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(36, 35), cv::Point2f(63, 35), cv::Scalar(255, 255, 255, 255), 1);
			return img;
		}

		cv::Mat ControlUnitImage()
		{
			cv::Mat img(64, 64, CV_8UC4);
			img = 0;
			// border
			cv::line(img, cv::Point2f(0, 0), cv::Point2f(63, 0), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 0), cv::Point2f(63, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 63), cv::Point2f(0, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(0, 63), cv::Point2f(0, 0), cv::Scalar(255, 255, 255, 255), 1);

			cv::line(img, cv::Point2f(5, 5), cv::Point2f(25, 5), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(25, 5), cv::Point2f(25, 15), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(25, 15), cv::Point2f(5, 15), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(5, 15), cv::Point2f(5, 5), cv::Scalar(255, 255, 255, 255), 2);


			cv::line(img, cv::Point2f(12, 25), cv::Point2f(32, 25), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(32, 25), cv::Point2f(32, 35), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(32, 35), cv::Point2f(12, 35), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(12, 35), cv::Point2f(12, 25), cv::Scalar(255, 255, 255, 255), 2);

			cv::line(img, cv::Point2f(35, 12), cv::Point2f(55, 12), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(55, 12), cv::Point2f(55, 22), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(55, 22), cv::Point2f(35, 22), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(35, 22), cv::Point2f(35, 12), cv::Scalar(255, 255, 255, 255), 2);

			cv::line(img, cv::Point2f(15, 15), cv::Point2f(12, 30), cv::Scalar(255, 255, 255, 255), 1);

			cv::line(img, cv::Point2f(45, 22), cv::Point2f(32, 30), cv::Scalar(255, 255, 255, 255), 1);

			cv::putText(img, cv::String("Control"), cv::Point2f(4, 50), 1, 0.9, cv::Scalar(255, 255, 255, 255));
			return img;
		}

		cv::Mat EmptyGridCellImage()
		{
			cv::Mat img(64, 64, CV_8UC4);
			img = 0;
			// border
			cv::line(img, cv::Point2f(0, 0), cv::Point2f(63, 0), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 0), cv::Point2f(63, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 63), cv::Point2f(0, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(0, 63), cv::Point2f(0, 0), cv::Scalar(255, 255, 255, 255), 1);

			return img;
		}

		cv::Mat DecoderImage()
		{
			cv::Mat img(64, 64, CV_8UC4);
			img = 0;
			// border
			cv::line(img, cv::Point2f(0, 0), cv::Point2f(63, 0), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 0), cv::Point2f(63, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(63, 63), cv::Point2f(0, 63), cv::Scalar(255, 255, 255, 255), 1);
			cv::line(img, cv::Point2f(0, 63), cv::Point2f(0, 0), cv::Scalar(255, 255, 255, 255), 1);

			cv::putText(img, cv::String("Decode"), cv::Point2f(4, 20), 1, 0.9, cv::Scalar(255, 255, 255, 255));

			cv::line(img, cv::Point2f(32, 23), cv::Point2f(32, 37), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(9, 37), cv::Point2f(54, 37), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(9, 37), cv::Point2f(9, 49), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(54, 37), cv::Point2f(54, 49), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(9, 49), cv::Point2f(54, 49), cv::Scalar(255, 255, 255, 255), 2);

			cv::line(img, cv::Point2f(17, 49), cv::Point2f(17, 57), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(27, 49), cv::Point2f(27, 57), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(37, 49), cv::Point2f(37, 57), cv::Scalar(255, 255, 255, 255), 2);
			cv::line(img, cv::Point2f(47, 49), cv::Point2f(47, 57), cv::Scalar(255, 255, 255, 255), 2);

			return img;
		}
	}
}
