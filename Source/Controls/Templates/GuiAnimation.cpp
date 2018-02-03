#include "GuiAnimation.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiTimedAnimation
***********************************************************************/

			class GuiTimedAnimation : public Object, public virtual IGuiAnimation
			{
			protected:
				DateTime						startTime;
				vuint64_t						time;
				bool							running = false;

			public:
				GuiTimedAnimation()
				{
				}

				~GuiTimedAnimation()
				{
				}

				void Start()override
				{
					startTime = DateTime::LocalTime();
					time = 0;
					running = true;
				}

				void Pause()override
				{
					time = GetTime();
					running = false;
				}

				void Resume()override
				{
					startTime = DateTime::LocalTime();
					running = true;
				}

				vuint64_t GetTime()
				{
					if (running)
					{
						return time + (DateTime::LocalTime().totalMilliseconds - startTime.totalMilliseconds);
					}
					else
					{
						return time;
					}
				}
			};

/***********************************************************************
GuiFiniteAnimation
***********************************************************************/

			class GuiFiniteAnimation : public GuiTimedAnimation
			{
			protected:
				vuint64_t						length = 0;
				Func<void(vuint64_t)>			run;

			public:
				GuiFiniteAnimation(const Func<void(vuint64_t)>& _run, vuint64_t _length)
					:run(_run)
					, length(_length)
				{
				}

				~GuiFiniteAnimation()
				{
				}

				void Run()override
				{
					auto currentTime = GetTime();
					if (currentTime < length && run)
					{
						run(currentTime);
					}
				}

				bool GetStopped()override
				{
					return GetTime() >= length;
				}
			};

/***********************************************************************
GuiInfiniteAnimation
***********************************************************************/

			class GuiInfiniteAnimation : public GuiTimedAnimation
			{
			protected:
				Func<void(vuint64_t)>			run;

			public:
				GuiInfiniteAnimation(const Func<void(vuint64_t)>& _run)
					:run(_run)
				{
				}

				~GuiInfiniteAnimation()
				{
				}

				void Run()override
				{
					if (run)
					{
						run(GetTime());
					}
				}

				bool GetStopped()override
				{
					return false;
				}
			};

/***********************************************************************
IGuiAnimation
***********************************************************************/

			Ptr<IGuiAnimation> IGuiAnimation::CreateAnimation(const Func<void(vuint64_t)>& run, vuint64_t milliseconds)
			{
				return new GuiFiniteAnimation(run, milliseconds);
			}

			Ptr<IGuiAnimation> IGuiAnimation::CreateAnimation(const Func<void(vuint64_t)>& run)
			{
				return new GuiInfiniteAnimation(run);
			}

/***********************************************************************
IGuiAnimationCoroutine
***********************************************************************/

			class GuiCoroutineAnimation : public Object, public virtual IGuiAnimationCoroutine::IImpl
			{
			protected:
				IGuiAnimationCoroutine::Creator				creator;

			public:
				GuiCoroutineAnimation(const IGuiAnimationCoroutine::Creator& _creator)
					:creator(_creator)
				{
				}

				~GuiCoroutineAnimation()
				{
				}

				void OnPlayAndWait(Ptr<IGuiAnimation> animation)override
				{
				}

				void OnPlayInGroup(Ptr<IGuiAnimation> animation, vint groupId)override
				{
				}

				void OnWaitForGroup(vint groupId)override
				{
				}

				void Start()override
				{
				}

				void Pause()override
				{
				}

				void Resume()override
				{
				}

				void Run()override
				{
				}

				bool GetStopped()override
				{
				}
			};

			void IGuiAnimationCoroutine::Wait(IImpl* impl, vuint64_t milliseconds)
			{
				return PlayAndWait(impl, IGuiAnimation::CreateAnimation({}, milliseconds));
			}

			void IGuiAnimationCoroutine::PlayAndWait(IImpl* impl, Ptr<IGuiAnimation> animation)
			{
				impl->OnPlayAndWait(animation);
			}

			void IGuiAnimationCoroutine::PlayInGroup(IImpl* impl, Ptr<IGuiAnimation> animation, vint groupId)
			{
				impl->OnPlayInGroup(animation, groupId);
			}

			void IGuiAnimationCoroutine::WaitForGroup(IImpl* impl, vint groupId)
			{
				impl->OnWaitForGroup(groupId);
			}

			Ptr<IGuiAnimation> IGuiAnimationCoroutine::Create(const Creator& creator)
			{
				return new GuiCoroutineAnimation(creator);
			}
		}
	}
}