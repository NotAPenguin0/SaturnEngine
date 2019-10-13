#ifndef SATURN_TIME_HPP_
#define SATURN_TIME_HPP_

namespace Saturn {

class Time {
public:
    static inline float deltaTime;

    static float now();
    static void update();
   
	static inline float lastFrame;
};

} // namespace Saturn

#endif