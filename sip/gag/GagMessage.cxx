#include "GagMessage.hxx"

// Yeah, this is kind of ugly, but I can't think of
// a good pattern for this off the top of my head.

GagMessage *
GagMessage::createMessage(istream &is)
{
  char messageType;
  is.get(messageType);

  switch (messageType)
  {
    case IM:
        return new GagImMessage(is);
      break;

    case PRESENCE:
        return new GagPresenceMessage(is);
      break;

    case LOGIN:
        return new GagLoginMessage(is);
      break;

    case LOGOUT:
        return new GagLogoutMessage(is);
      break;

    case ADD_BUDDY:
        return new GagAddBuddyMessage(is);
      break;

    case REMOVE_BUDDY:
        return new GagRemoveBuddyMessage(is);
      break;

    // Technically, we shouldn't get these. But, oh well.
    case ERROR:
        return new GagErrorMessage(is);
      break;

    default:
      // XXX Something is HORRIBLY WRONG
        return 0;
      break;
  }
}

// Helper functions for I/O

void
GagMessage::serialize(ostream &os, const Data& data)
{
  os.write((char *)(int)data.size(), sizeof(int));
  os.write(data.data(), data.size());
}

void
GagMessage::serialize(ostream &os, const Uri& uri)
{
  serialize(os, uri.getAor());
}

void
GagMessage::serialize(ostream &os, const bool& flag)
{
  os.write((char *)((int)1), sizeof(int));
  os.put(flag?0:1);
}

bool
GagMessage::parse(istream &is, Data &data)
{
  int size;
  char *temp;

  is.get((char *)size, sizeof(size));
  temp=(char *)malloc(size);
  if (!temp) return (false);
  is.get(temp, size);
  data = Data(temp);
  free(temp);
  return true;
}

bool
GagMessage::parse(istream &is, Uri &uri)
{
  int size;
  char *temp;

  is.get((char *)size, sizeof(size));
  temp=(char *)malloc(size);
  if (!temp) return (false);
  is.get(temp, size);
  uri = Uri(temp);
  free(temp);
  return true;
}

bool
GagMessage::parse(istream &is, bool &flag)
{
  int size;
  char *temp;

  is.get((char *)size, sizeof(size));
  temp=(char *)malloc(size);
  if (!temp) return (false);
  is.get(temp, size);
  flag = (temp[0]?true:false);
  free(temp);
  return true;
}

void
GagImMessage::parse(istream &is)
{
  valid = false;
  if (!GagMessage::parse(is, to)) return;
  if (!GagMessage::parse(is, from)) return;
  if (!GagMessage::parse(is, im)) return;
  valid = true;
}

ostream &
GagImMessage::serialize(ostream &os) const
{
  GagMessage::serialize(os, to);
  GagMessage::serialize(os, from);
  GagMessage::serialize(os, im);

  return os;
}

void
GagPresenceMessage::parse(istream &is)
{
  valid = false;
  if (!GagMessage::parse(is, aor)) return;
  if (!GagMessage::parse(is, available)) return;
  if (!GagMessage::parse(is, status)) return;
  valid = true;
}

ostream &
GagPresenceMessage::serialize(ostream &os) const
{
  GagMessage::serialize(os, aor);
  GagMessage::serialize(os, available);
  GagMessage::serialize(os, status);

  return os;
}

void
GagLoginMessage::parse(istream &is)
{
  valid = false;
  if (!GagMessage::parse(is, aor)) return;
  if (!GagMessage::parse(is, userid)) return;
  if (!GagMessage::parse(is, password)) return;
  valid = true;
}

ostream &
GagLoginMessage::serialize(ostream &os) const
{
  GagMessage::serialize(os, aor);
  GagMessage::serialize(os, userid);
  GagMessage::serialize(os, password);

  return os;
}

void
GagLogoutMessage::parse(istream &is)
{
  valid = false;
  if (!GagMessage::parse(is, aor)) return;
  valid = true;
}

ostream &
GagLogoutMessage::serialize(ostream &os) const
{
  GagMessage::serialize(os, aor);
  return os;
}

void
GagAddBuddyMessage::parse(istream &is)
{
  valid = false;
  if (!GagMessage::parse(is, buddy)) return;
  valid = true;
}

ostream &
GagAddBuddyMessage::serialize(ostream &os) const
{
  GagMessage::serialize(os, buddy);
  return os;
}


void
GagRemoveBuddyMessage::parse(istream &is)
{
  valid = false;
  if (!GagMessage::parse(is, buddy)) return;
  valid = true;
}

ostream &
GagRemoveBuddyMessage::serialize(ostream &os) const
{
  GagMessage::serialize(os, buddy);
  return os;
}


void
GagErrorMessage::parse(istream &is)
{
  valid = false;
  if (!GagMessage::parse(is, message)) return;
  valid = true;
}

ostream &
GagErrorMessage::serialize(ostream &os) const
{
  GagMessage::serialize(os, message);
  return os;
}


