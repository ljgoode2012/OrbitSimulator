/***********************************************************************
 * Source File:
 *    Satellite : The representation of a satellite in orbit
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Everything we need to know about a satellite in orbit
 ************************************************************************/

#include "satellite.h"
#include "constants.h"
#include "uiDraw.h"
#include <cmath>
#include <cstdlib>
#include <vector>

namespace
{
   /******************************************
    * Compute Earth Facing Rotation
    * Calculate the rotation angle needed to face Earth
    *****************************************/
   Angle computeEarthFacingRotation(const Position& position)
   {
      Angle earthFacingRotation;
      earthFacingRotation.setDxDy(-position.getMetersX(),
                                  -position.getMetersY());
      earthFacingRotation.addRadians(SATELLITE_COUNTERCLOCKWISE_OFFSET_RADIANS);
      return earthFacingRotation;
   }

   /******************************************
    * Random Double
    * Generate a random double between min and max
    *****************************************/
   double randomDouble(double min, double max)
   {
      return min + (static_cast<double>(std::rand()) /
                    static_cast<double>(RAND_MAX)) *
                      (max - min);
   }

   /******************************************
    * Create Debris Velocity
    * Generate a random velocity for debris based on parent entity
    *****************************************/
   Velocity createDebrisVelocity(const Entity& parent)
   {
      const double directionRadians = randomDouble(0.0, 2.0 * M_PI);
      const double kick = randomDouble(KICK_MIN_METERS_PER_SECOND,
                                       KICK_MAX_METERS_PER_SECOND);

      return Velocity(
         parent.getVelocityDX() + std::sin(directionRadians) * kick,
         parent.getVelocityDY() + std::cos(directionRadians) * kick);
   }

   /******************************************
    * Create Debris Position
    * Calculate spawn position for debris offset from parent
    *****************************************/
   Position createDebrisPosition(const Entity& parent,
                                 const Velocity& debrisVelocity)
   {
      Position spawnPosition = parent.getPosition();
      Angle travelDirection;
      travelDirection.setDxDy(debrisVelocity.getDX(), debrisVelocity.getDY());
      spawnPosition.addPixelsX(travelDirection.getDx() * SPAWN_OFFSET_PIXELS);
      spawnPosition.addPixelsY(travelDirection.getDy() * SPAWN_OFFSET_PIXELS);
      return spawnPosition;
   }

   /******************************************
    * Create Fragment Expire Time Seconds
    * Generate a random lifetime for a fragment
    *****************************************/
   double createFragmentExpireTimeSeconds()
   {
      const double frames = randomDouble(FRAGMENT_MIN_LIFETIME_FRAMES,
                                         FRAGMENT_MAX_LIFETIME_FRAMES);
      return frames * SIM_SECONDS_PER_FRAME;
   }

   /******************************************
    * Create Random Rotation
    * Generate a random rotation angle
    *****************************************/
   Angle createRandomRotation()
   {
      return Angle(randomDouble(0.0, 2.0 * M_PI));
   }

   /******************************************
    * Create Fragment Spin Rate
    * Generate a random spin rate with random direction
    *****************************************/
   double createFragmentSpinRate()
   {
      const double magnitude = randomDouble(
         FRAGMENT_MIN_SPIN_RADIANS_PER_SECOND,
         FRAGMENT_MAX_SPIN_RADIANS_PER_SECOND);
      return (std::rand() % 2 == 0) ? magnitude : -magnitude;
   }

   /******************************************
    * Add Part From Entity
    * Create a satellite part from parent entity and add to debris list
    *****************************************/
   void addPartFromEntity(const Entity& parent,
                          SatellitePart::DrawType drawType, double radiusPixels,
                          int fragmentsOnBreak,
                          std::vector<std::unique_ptr<Entity>>& debrisOut)
   {
      const Velocity debrisVelocity = createDebrisVelocity(parent);
      const Position spawnPosition = createDebrisPosition(parent,
                                                          debrisVelocity);
      std::unique_ptr<SatellitePart> part(new SatellitePart(
         spawnPosition, debrisVelocity, createRandomRotation(), drawType,
         radiusPixels, fragmentsOnBreak));
      debrisOut.push_back(std::move(part));
   }
} // namespace

/******************************************
 * SatellitePart : Update
 * Update the satellite part and handle collision immunity
 *****************************************/
void SatellitePart::update(double dt)
{
   Entity::update(dt);
   if (collisionImmunityFrames > 0)
      --collisionImmunityFrames;
}

/******************************************
 * Satellite : Draw
 * Draw a generic satellite; concrete types should override this.
 *****************************************/
void Satellite::draw(ogstream& gout) const
{
   (void)gout;
}

/******************************************
 * Satellite : Create Breakup Debris
 * Generate debris when satellite breaks up (base implementation)
 *****************************************/
void Satellite::createBreakupDebris(
   std::vector<std::unique_ptr<Entity>>& debris) const
{
   (void)debris;
}

/******************************************
 * Fragment : Draw
 * Draw a fragment
 *****************************************/
void Fragment::draw(ogstream& gout) const
{
   gout.drawFragment(getPosition(), getRotation());
}

/******************************************
 * Fragment : Update
 * Update the fragment and handle collision immunity
 *****************************************/
void Fragment::update(double dt)
{
   TimedEntity::update(dt);
   if (collisionImmunityFrames > 0)
      --collisionImmunityFrames;
}

/******************************************
 * SatellitePart : Constructor
 * Initialize a satellite part with position, velocity, and visual properties
 *****************************************/
SatellitePart::SatellitePart(const Position& pos, const Velocity& vel,
                             const Angle& rotation, DrawType drawType,
                             double radiusPixels, int fragmentsOnBreak)
    : BreakableEntity(pos, vel), drawType(drawType), radiusPixels(radiusPixels),
      fragmentsOnBreak(fragmentsOnBreak),
      collisionImmunityFrames(COLLISION_IMMUNITY_FRAMES)
{
   setRotation(rotation);
}

/******************************************
 * SatellitePart : Draw
 * Draw the appropriate satellite part based on type
 *****************************************/
void SatellitePart::draw(ogstream& gout) const
{
   switch (drawType)
   {
   case STARLINK_BODY:
      gout.drawStarlinkBody(getPosition(), getRotation());
      break;
   case STARLINK_ARRAY:
      gout.drawStarlinkArray(getPosition(), getRotation());
      break;
   case CREW_DRAGON_CENTER:
      gout.drawCrewDragonCenter(getPosition(), getRotation());
      break;
   case CREW_DRAGON_LEFT:
      gout.drawCrewDragonLeft(getPosition(), getRotation());
      break;
   case CREW_DRAGON_RIGHT:
      gout.drawCrewDragonRight(getPosition(), getRotation());
      break;
   case HUBBLE_TELESCOPE:
      gout.drawHubbleTelescope(getPosition(), getRotation());
      break;
   case HUBBLE_COMPUTER:
      gout.drawHubbleComputer(getPosition(), getRotation());
      break;
   case HUBBLE_LEFT:
      gout.drawHubbleLeft(getPosition(), getRotation());
      break;
   case HUBBLE_RIGHT:
      gout.drawHubbleRight(getPosition(), getRotation());
      break;
   case GPS_CENTER:
      gout.drawGPSCenter(getPosition(), getRotation());
      break;
   case GPS_LEFT:
      gout.drawGPSLeft(getPosition(), getRotation());
      break;
   case GPS_RIGHT:
      gout.drawGPSRight(getPosition(), getRotation());
      break;
   default:
      break;
   }
}

/******************************************
 * Satellite : Set Is Defunct
 * Mark satellite as defunct and set random spin or stop spinning
 *****************************************/
void Satellite::setIsDefunct(bool isDefunct)
{
   this->isDefunct = isDefunct;
   if (this->isDefunct)
      setRandomSpinRate();
   else
      setAngularVelocity(0.0);
}

/******************************************
 * Satellite : Random Spin Rate
 * Initialize the angular velocity to a random spin rate.
 *****************************************/
void Satellite::setRandomSpinRate()
{
   const double spinRate =
      MIN_SATELLITE_SPIN_RATE +
      (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)) *
         (MAX_SATELLITE_SPIN_RATE - MIN_SATELLITE_SPIN_RATE);
   const double direction = (std::rand() % 2 == 0) ? 1.0 : -1.0;
   setAngularVelocity(direction * spinRate);
}

/******************************************
 * Satellite : Initialize Circular Orbit
 * Set this satellite's initial position and velocity for a circular orbit.
 *****************************************/
void Satellite::initializeCircularOrbit(double orbitalRadiusMeters,
                                        double phaseRadians)
{
   if (orbitalRadiusMeters <= 0.0)
      return;

   const double speed = std::sqrt(MU / orbitalRadiusMeters);
   const double x = orbitalRadiusMeters * std::cos(phaseRadians);
   const double y = orbitalRadiusMeters * std::sin(phaseRadians);
   const double vx = -speed * std::sin(phaseRadians);
   const double vy = speed * std::cos(phaseRadians);

   Position position;
   position.setMeters(x, y);
   setPosition(position);
   setVelocity(Velocity(vx, vy));
   setRotation(computeEarthFacingRotation(position));
}

/******************************************
 * Satellite : Update
 * Update the satellite's position and rotation
 *****************************************/
void Satellite::update(double dt)
{
   Entity::update(dt);

   if (!getIsDefunct())
   {
      const Position& position = getPosition();
      const double xMeters = position.getMetersX();
      const double yMeters = position.getMetersY();

      if (xMeters != 0.0 || yMeters != 0.0)
         setRotation(computeEarthFacingRotation(position));
   }
}

/******************************************
 * Hubble : Draw
 * Draw the Hubble telescope
 *****************************************/
void Hubble::draw(ogstream& gout) const
{
   gout.drawHubble(getPosition(), getRotation());
}

/******************************************
 * Starlink : Draw
 * Draw the Starlink satellite
 *****************************************/
void Starlink::draw(ogstream& gout) const
{
   gout.drawStarlink(getPosition(), getRotation());
}

/******************************************
 * CrewDragon : Draw
 * Draw the Crew Dragon spacecraft
 *****************************************/
void CrewDragon::draw(ogstream& gout) const
{
   gout.drawCrewDragon(getPosition(), getRotation());
}

/******************************************
 * GPS : Draw
 * Draw the GPS satellite
 *****************************************/
void GPS::draw(ogstream& gout) const
{
   gout.drawGPS(getPosition(), getRotation());
}

/******************************************
 * Sputnik : Draw
 * Draw the Sputnik satellite
 *****************************************/
void Sputnik::draw(ogstream& gout) const
{
   gout.drawSputnik(getPosition(), getRotation());
}

/******************************************
 * Create Fragments From Entity
 * Generate multiple fragments from a parent entity
 *****************************************/
void createFragmentsFromEntity(const Entity& parent, int fragmentCount,
                               std::vector<std::unique_ptr<Entity>>& debrisOut)
{
   for (int i = 0; i < fragmentCount; ++i)
   {
      const Velocity debrisVelocity = createDebrisVelocity(parent);
      const Position spawnPosition = createDebrisPosition(parent,
                                                          debrisVelocity);
      std::unique_ptr<Fragment> fragment(new Fragment(
         spawnPosition, debrisVelocity, createFragmentExpireTimeSeconds()));
      fragment->setInitialRotation(createRandomRotation());
      fragment->setAngularVelocity(createFragmentSpinRate());
      debrisOut.push_back(std::move(fragment));
   }
}

/******************************************
 * Hubble : Create Breakup Debris
 * Generate debris when Hubble breaks up
 *****************************************/
void Hubble::createBreakupDebris(
   std::vector<std::unique_ptr<Entity>>& debris) const
{
   addPartFromEntity(*this, SatellitePart::HUBBLE_TELESCOPE,
                     HUBBLE_TELESCOPE_RADIUS_PIXELS, HUBBLE_TELESCOPE_FRAGMENTS,
                     debris);
   addPartFromEntity(*this, SatellitePart::HUBBLE_COMPUTER,
                     HUBBLE_COMPUTER_RADIUS_PIXELS, HUBBLE_COMPUTER_FRAGMENTS,
                     debris);
   addPartFromEntity(*this, SatellitePart::HUBBLE_LEFT,
                     HUBBLE_LEFT_RADIUS_PIXELS, HUBBLE_LEFT_FRAGMENTS, debris);
   addPartFromEntity(*this, SatellitePart::HUBBLE_RIGHT,
                     HUBBLE_RIGHT_RADIUS_PIXELS, HUBBLE_RIGHT_FRAGMENTS,
                     debris);
}

/******************************************
 * Starlink : Create Breakup Debris
 * Generate debris when Starlink breaks up
 *****************************************/
void Starlink::createBreakupDebris(
   std::vector<std::unique_ptr<Entity>>& debris) const
{
   addPartFromEntity(*this, SatellitePart::STARLINK_BODY,
                     STARLINK_BODY_RADIUS_PIXELS, STARLINK_BODY_FRAGMENTS,
                     debris);
   addPartFromEntity(*this, SatellitePart::STARLINK_ARRAY,
                     STARLINK_ARRAY_RADIUS_PIXELS, STARLINK_ARRAY_FRAGMENTS,
                     debris);
   createFragmentsFromEntity(*this, STARLINK_EXTRA_FRAGMENTS, debris);
}

/******************************************
 * CrewDragon : Create Breakup Debris
 * Generate debris when Crew Dragon breaks up
 *****************************************/
void CrewDragon::createBreakupDebris(
   std::vector<std::unique_ptr<Entity>>& debris) const
{
   addPartFromEntity(*this, SatellitePart::CREW_DRAGON_CENTER,
                     CREW_DRAGON_CENTER_RADIUS_PIXELS,
                     CREW_DRAGON_CENTER_FRAGMENTS, debris);
   addPartFromEntity(*this, SatellitePart::CREW_DRAGON_LEFT,
                     CREW_DRAGON_LEFT_RADIUS_PIXELS, CREW_DRAGON_LEFT_FRAGMENTS,
                     debris);
   addPartFromEntity(*this, SatellitePart::CREW_DRAGON_RIGHT,
                     CREW_DRAGON_RIGHT_RADIUS_PIXELS,
                     CREW_DRAGON_RIGHT_FRAGMENTS, debris);
   createFragmentsFromEntity(*this, CREW_DRAGON_EXTRA_FRAGMENTS, debris);
}

/******************************************
 * GPS : Create Breakup Debris
 * Generate debris when GPS breaks up
 *****************************************/
void GPS::createBreakupDebris(
   std::vector<std::unique_ptr<Entity>>& debris) const
{
   addPartFromEntity(*this, SatellitePart::GPS_CENTER, GPS_CENTER_RADIUS_PIXELS,
                     GPS_CENTER_FRAGMENTS, debris);
   addPartFromEntity(*this, SatellitePart::GPS_LEFT, GPS_LEFT_RADIUS_PIXELS,
                     GPS_LEFT_FRAGMENTS, debris);
   addPartFromEntity(*this, SatellitePart::GPS_RIGHT, GPS_RIGHT_RADIUS_PIXELS,
                     GPS_RIGHT_FRAGMENTS, debris);
   createFragmentsFromEntity(*this, GPS_EXTRA_FRAGMENTS, debris);
}

/******************************************
 * Sputnik : Create Breakup Debris
 * Generate debris when Sputnik breaks up
 *****************************************/
void Sputnik::createBreakupDebris(
   std::vector<std::unique_ptr<Entity>>& debris) const
{
   createFragmentsFromEntity(*this, SPUTNIK_FRAGMENTS, debris);
}

/******************************************
 * SatellitePart : Create Breakup Debris
 * Generate debris when a satellite part breaks up
 *****************************************/
void SatellitePart::createBreakupDebris(
   std::vector<std::unique_ptr<Entity>>& debris) const
{
   createFragmentsFromEntity(*this, fragmentsOnBreak, debris);
}
