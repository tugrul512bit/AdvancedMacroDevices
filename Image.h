#pragma once
#include "AppStructure.h"
#include<functional>
#include<vector>
#include<string>
#include<opencv2/opencv.hpp>
#include <GL/glcorearb.h>
namespace Window
{
	class ImageItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, bool sameLine, int width, int height)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<ImageItem>(new ImageItem(name, sameLine, width, height), [](ImageItem* i) { delete i; });
			return node;
		}

		ImageItem(std::string name = "text", bool sameLine = false, int width=64, int height=64)
		{
			_name = name;
			_sameLine = sameLine;
			_img = cv::Mat(height, width, CV_8UC4);
			for (int i = 0; i < height * width * 4; i++)
				_img.at<unsigned char>(i) = 155;
			cv::circle(_img, cv::Point2f(32, 32), 15, cv::Scalar(255, 255, 255,255),2);
			cv::line(_img, cv::Point2f(0, 32), cv::Point2f(64, 32), cv::Scalar(255, 255, 255, 255), 4);
			_width = width;
			_height = height;
			_vec1 = ImVec2(_width, _height);
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

        GLuint ToTexture() {
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
				_width,
				_height,
                0,                 
                inputColourFormat, 
                GL_UNSIGNED_BYTE,  
                _img.data);        
            return textureID;
        }
	};
}
