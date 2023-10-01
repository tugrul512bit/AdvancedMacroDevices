#pragma once
#include<opencv2/opencv.hpp>
#include<queue>
namespace Global
{
	namespace ImageProcessing
	{
		cv::Mat FloodFill(int x, int y, cv::Mat img, unsigned char color1, unsigned char color2, unsigned char color3, unsigned char color4)
		{
			cv::Mat result = img.clone();

			const int xMax = img.cols;
			const int yMax = img.rows;
			struct Point
			{
				int x, y;
				Point(int xPrm, int yPrm):x(xPrm),y(yPrm) {}
			};
			std::queue<Point> q;
			q.push(Point(x, y));

			while (!q.empty())
			{
				Point pt = q.front();
				q.pop();

				if (pt.x >= 0 && pt.x < xMax && pt.y >= 0 && pt.y < yMax)
				{
					if (result.at<unsigned char>((pt.x + pt.y * xMax) * 4) != color1 ||
						result.at<unsigned char>((pt.x + pt.y * xMax) * 4 + 1) != color2 ||
						result.at<unsigned char>((pt.x + pt.y * xMax) * 4 + 2) != color3 ||
						result.at<unsigned char>((pt.x + pt.y * xMax) * 4 + 3) != color4
						)
					{
						result.at<unsigned char>((pt.x + pt.y * xMax) * 4) = color1;
						result.at<unsigned char>((pt.x + pt.y * xMax) * 4+1) = color2;
						result.at<unsigned char>((pt.x + pt.y * xMax) * 4+2) = color3;
						result.at<unsigned char>((pt.x + pt.y * xMax) * 4+3) = color4;


						if (pt.x + 1 >= 0 && pt.x + 1 < xMax && pt.y >= 0 && pt.y < yMax)
						{
							if (result.at<unsigned char>((pt.x + 1 + pt.y * xMax) * 4) != color1 ||
								result.at<unsigned char>((pt.x + 1 + pt.y * xMax) * 4 + 1) != color2 ||
								result.at<unsigned char>((pt.x + 1 + pt.y * xMax) * 4 + 2) != color3 ||
								result.at<unsigned char>((pt.x + 1 + pt.y * xMax) * 4 + 3) != color4
								)
								q.push(Point(pt.x + 1, pt.y));
						}
						if (pt.x - 1 >= 0 && pt.x - 1 < xMax && pt.y >= 0 && pt.y < yMax)
						{
							if (result.at<unsigned char>((pt.x - 1 + pt.y * xMax) * 4) != color1 ||
								result.at<unsigned char>((pt.x - 1 + pt.y * xMax) * 4 + 1) != color2 ||
								result.at<unsigned char>((pt.x - 1 + pt.y * xMax) * 4 + 2) != color3 ||
								result.at<unsigned char>((pt.x - 1 + pt.y * xMax) * 4 + 3) != color4
								)
								q.push(Point(pt.x - 1, pt.y));
						}
						if (pt.x >= 0 && pt.x < xMax && pt.y + 1 >= 0 && pt.y + 1 < yMax)
						{
							if (result.at<unsigned char>((pt.x + pt.y * xMax + xMax) * 4) != color1 ||
								result.at<unsigned char>((pt.x + pt.y * xMax + xMax) * 4 + 1) != color2 ||
								result.at<unsigned char>((pt.x + pt.y * xMax + xMax) * 4 + 2) != color3 ||
								result.at<unsigned char>((pt.x + pt.y * xMax + xMax) * 4 + 3) != color4
								)
								q.push(Point(pt.x, pt.y + 1));
						}
						if (pt.x >= 0 && pt.x < xMax && pt.y - 1 >= 0 && pt.y - 1 < yMax)
						{
							if (result.at<unsigned char>((pt.x + pt.y * xMax - xMax) * 4) != color1 ||
								result.at<unsigned char>((pt.x + pt.y * xMax - xMax) * 4 + 1) != color2 ||
								result.at<unsigned char>((pt.x + pt.y * xMax - xMax) * 4 + 2) != color3 ||
								result.at<unsigned char>((pt.x + pt.y * xMax - xMax) * 4 + 3) != color4
								)
								q.push(Point(pt.x, pt.y - 1));
						}
					}
				}
			}
			return result;
		}
	}
}