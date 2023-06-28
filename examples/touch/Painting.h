
class Painting : public Solid
{
public:
    using Solid::Solid; // inherit constructors

    bool touchDownHandler(TouchContext *ctx) override
    {
        if (ctx->containsCurrent())
        {
            log_i("starting line");
            _lines.push_back(std::vector<Point>());
            _prior = ctx->current();
            touchCapture(true);
            return true;
        }
        return false;
    }

    bool touchMoveHandler(TouchContext *ctx) override
    {
        if (touchCapture())
        {
            if (ctx->containsCurrent())
            {
                Point pt = ctx->current();
                log_i("adding point {%d,%d}", pt.x(), pt.y());
                _lines.rbegin()->push_back(pt);
                invalidate(Rect(
                    _min(pt.x(), _prior.x()),
                    _min(pt.y(), _prior.y()),
                    _max(pt.x(), _prior.x()) + 1,
                    _max(pt.y(), _prior.y()) + 1));
                _prior = ctx->current();
                return true;
            }
            touchRelease();
        }
        return false;
    }

    bool touchUpHandler(TouchContext *ctx) override
    {
        if (touchCapture())
        {
            touchRelease();
            return true;
        }
        return false;
    }

    void drawHandler(DrawContext *ctx) override
    {
        Rect rc = content();
        ctx->draw(rc, style()->background());
        for (const auto &line : _lines)
        {
            bool first = true;
            Point prior;
            for (Point pt : line)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    ctx->draw(prior, pt, style()->foreground());
                }
                prior = pt;
            }
        }
        ctx->exclude(rc);
    }

private:
    std::vector<std::vector<Point>> _lines;
    Point _prior;
};

