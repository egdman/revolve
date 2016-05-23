#include "SoundPlugin.h"
#include <iostream>

namespace gz = gazebo;

namespace revolve {
namespace gazebo {
	
	
SoundPlugin::SoundPlugin():
	soundSourcesPosePubFreq_(0),
	lastPubTime_(0)
{
}


void SoundPlugin::Load(::gz::physics::WorldPtr _parent, sdf::ElementPtr _sdf)
{
	// store the world
	world_ = _parent;
	
	// create the transport node
	node_.reset(new gz::transport::Node());
	node_->Init();

    requestSub_ = node_->Subscribe("~/request", &SoundPlugin::OnRequest, this);
	responsePub_ = node_->Advertise<gz::msgs::Response>("~/response");
	
	// sound source pose publisher
	SoundSourcePosesPub_ = node_->Advertise<gz::msgs::PosesStamped>("~/revolve/sound_source_poses", 50);
	
	// Bind to the world update event to OnUpdate method
	updateConnection_ = gz::event::Events::ConnectWorldUpdateBegin(
			boost::bind(&SoundPlugin::OnUpdate, this, _1));
	
	std::cout << "Sound plugin loaded." << std::endl;
}

void SoundPlugin::OnRequest(ConstRequestPtr & _msg)
{
	if (_msg->request() == "set_sound_source_update_frequency") {
        soundSourcesPosePubFreq_ = boost::lexical_cast<double>(_msg->data());
		std::cout << "Sound plugin: update frequency set to " << soundSourcesPosePubFreq_ << std::endl;
		
		// publish response about successful handling:
		gz::msgs::Response resp;
		resp.set_id(_msg->id());
		resp.set_request(_msg->request());
		resp.set_response("success");
		responsePub_->Publish(resp);
		
	}
	else if (_msg->request() == "add_sound_source") {
        double frequency = boost::lexical_cast<double>(_msg->dbl_data());
		std::string sourceName = _msg->data();
		
		sourceNamesMutex_.lock();
        soundSourceNames_[sourceName] = frequency;
		sourceNamesMutex_.unlock();
		
		// publish response about successful handling:
		gz::msgs::Response resp;
		resp.set_id(_msg->id());
		resp.set_request(_msg->request());
		resp.set_response("success");
		responsePub_->Publish(resp);
		
		// FOR DEBUG; this should be gone in the final version:
        std::cout << "Sound source added: " << sourceName << ", frequency: " << frequency << std::endl;
	}
}

void SoundPlugin::OnUpdate(const ::gz::common::UpdateInfo &_info)
{
	if (soundSourcesPosePubFreq_ == 0) {
		return;
	}
    double secs = 1.0 / soundSourcesPosePubFreq_;
	double time = _info.simTime.Double();
	

	if ((time - lastPubTime_) >= secs) {
		
		gz::msgs::PosesStamped msg;
		gz::msgs::Set(msg.mutable_time(), _info.simTime);
			
		sourceNamesMutex_.lock();
		if (!soundSourceNames_.empty()) {
            for (	std::map<std::string, double>::iterator it = soundSourceNames_.begin(); it != soundSourceNames_.end(); ++it ) {
				std::string name = it->first;
                double frequency = it->second;
				gz::physics::ModelPtr model = world_->GetModel(name);
				 
				gz::msgs::Pose *poseMsg = msg.add_pose();
				poseMsg->set_name(model->GetScopedName());
				poseMsg->set_id(model->GetId());
				
				gz::math::Pose modelPose = model->GetWorldPose();
                // pose must be converted to an ignition::math::Pose3d object for some reason
				gz::msgs::Set(poseMsg, modelPose.Ign());
				
//				// FOR DEBUG; this should be gone in the final version:
//				std::cout << "time: " << time << ", source at: " << modelPose.pos.x << "," << modelPose.pos.y << "," << modelPose.pos.z << std::endl;
			}
		}
		sourceNamesMutex_.unlock();

		if (msg.pose_size() > 0) {
            SoundSourcePosesPub_->Publish(msg);
            lastPubTime_ = time;
        }
	}
	
}

} // namespace gazebo
} // namespace revolve

