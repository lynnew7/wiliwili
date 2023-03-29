//
// Created by fang on 2022/5/2.
//

#pragma once

#include "nlohmann/json.hpp"
#include "user_result.h"
#include "mine_result.h"

namespace bilibili {

/// Video Comment

// todo：up主精选评论

typedef std::unordered_map<std::string, std::string> VideoCommentEmoteMap;

class VideoCommentContent {
public:
    // 未初始化貌似会导致VideoCommentContent释放时候在switch上报错
    //    VideoCommentEmoteMap emote;
    std::string message;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoCommentContent, message);

class VideoCommentControl {
    /** eg
    {   "sub_reply_entry_text": "共192条回复",
        "sub_reply_title_text": "相关回复共192条",
        "time_desc": "22天前发布",
        "location": "IP属地：湖南" }
     */
public:
    std::string sub_reply_entry_text;
    std::string sub_reply_title_text;
    std::string time_desc;
    std::string location;
    bool up_like;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoCommentControl& nlohmann_json_t) {
    if (nlohmann_json_j.contains("location") &&
        !nlohmann_json_j.at("location").is_null()) {
        nlohmann_json_j.at("location").get_to(nlohmann_json_t.location);
    }
}

class LevelInfo {
public:
    LevelInfo() { current_level = 0; }
    int current_level;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LevelInfo, current_level);

class CommentUserResult {
public:
    CommentUserResult() {
        is_uploader      = false;
        is_senior_member = 0;
    }

    std::string mid, uname, avatar;
    int is_senior_member;
    LevelInfo level_info{};
    bool is_uploader;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CommentUserResult, mid, uname, avatar,
                                   is_senior_member, level_info);

class VideoCommentResult {
public:
    size_t ctime;
    int64_t rpid, parent, root;
    size_t oid;
    CommentUserResult member;
    VideoCommentContent content;
    std::vector<VideoCommentResult> replies;
    VideoCommentControl reply_control;
    size_t rcount, like, action;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoCommentResult& nlohmann_json_t) {
    if (!nlohmann_json_j.at("replies").is_null()) {
        nlohmann_json_j.at("replies").get_to(nlohmann_json_t.replies);
    }
    if (nlohmann_json_j.contains("member")) {
        nlohmann_json_j.at("member").get_to(nlohmann_json_t.member);
    }
    if (nlohmann_json_j.contains("reply_control") &&
        !nlohmann_json_j.at("reply_control").is_null()) {
        nlohmann_json_j.at("reply_control")
            .get_to(nlohmann_json_t.reply_control);
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, ctime, content,
                                             rcount, like, rpid, parent, root,
                                             oid, action));
}
typedef std::vector<VideoCommentResult> VideoCommentListResult;

class VideoCommentCursor {
public:
    int all_count = 0;
    int mode;  // 3: 热门评论 2: 按时间排序
    int next;
    int prev;
    bool is_end;
    bool is_begin;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoCommentCursor& nlohmann_json_t) {
    if (nlohmann_json_j.contains("all_count")) {
        nlohmann_json_j.at("all_count").get_to(nlohmann_json_t.all_count);
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, mode, next,
                                             is_end, is_begin, prev));
}

class VideoCommentResultWrapper {
public:
    VideoCommentCursor cursor;
    VideoCommentListResult replies;
    VideoCommentListResult top_replies;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoCommentResultWrapper& nlohmann_json_t) {
    if (!nlohmann_json_j.at("top_replies").is_null()) {
        nlohmann_json_j.at("top_replies").get_to(nlohmann_json_t.top_replies);
    }
    if (!nlohmann_json_j.at("replies").is_null()) {
        nlohmann_json_j.at("replies").get_to(nlohmann_json_t.replies);
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, cursor));
}

class VideoSingleCommentDetail {
public:
    VideoCommentCursor cursor;
    VideoCommentResult root;
    int64_t upper;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoSingleCommentDetail& nlohmann_json_t) {
    if (nlohmann_json_j.contains("upper") &&
        !nlohmann_json_j.at("upper").is_null()) {
        nlohmann_json_j.at("upper").at("mid").get_to(nlohmann_json_t.upper);
    } else {
        nlohmann_json_t.upper = 0;
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, cursor, root));
}

class VideoCommentAddResult {
public:
    int success_action;
    VideoCommentResult reply;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoCommentAddResult& nlohmann_json_t) {
    NLOHMANN_JSON_EXPAND(
        NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, success_action, reply));
}

/// Video Page

class VideoDetailPage {
public:
    int cid = 0;
    size_t page;        // 分p的序号
    int duration;       // 视频长度，单位秒
    int progress = -1;  // 视频初始化的播放时间，用于加载历史记录
    std::string part;  //标题
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoDetailPage, cid, page, duration, part);

typedef std::vector<VideoDetailPage> VideoDetailPageListResult;

class VideoDetailStat {
public:
    unsigned int aid;
    unsigned int view;
    unsigned int danmaku;
    unsigned int favorite;
    unsigned int coin;
    unsigned int share;
    unsigned int like;
    unsigned int reply;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoDetailStat, aid, view, danmaku,
                                   favorite, coin, share, like, reply);

class VideoDetailRights {
public:
    int download;
    int no_reprint;
    int is_cooperation;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoDetailRights, download, no_reprint,
                                   is_cooperation);

class UGCSeasonState {
public:
    int view;  // 播放量
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UGCSeasonState, view);

class UGCSeasonEpisodePage {
public:
    int cid, duration, page;
    std::string part;  // 原视频标题
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UGCSeasonEpisodePage, cid, duration, part,
                                   page);

class UGCSeasonEpisode {
public:
    int season_id = 0, section_id = 0, id = 0, aid = 0, cid = 0;
    int index = -1;
    std::string bvid, title;  // 合集视频标题
    UGCSeasonEpisodePage page;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UGCSeasonEpisode, season_id, section_id, id,
                                   aid, cid, title, bvid, page);

class UGCSeasonSection {
public:
    int season_id;
    int id;  // section id
    std::string title;
    int type;
    std::vector<UGCSeasonEpisode> episodes;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      UGCSeasonSection& nlohmann_json_t) {
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, season_id, id,
                                             type, title, episodes));
    for (int i = 0; i < nlohmann_json_t.episodes.size(); i++) {
        nlohmann_json_t.episodes[i].index = i;
    }
}
inline void to_json(nlohmann::json& nlohmann_json_j,
                    const UGCSeasonSection& nlohmann_json_t) {
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, season_id, id,
                                             type, title, episodes))
}

class UGCSeason {
public:
    int id;
    int64_t mid;
    std::string title, cover, intro;
    UGCSeasonState stat;
    bool is_pay_season;
    std::vector<UGCSeasonSection> sections;

    /// 以下项为前端显示所需，非原始接口数据

    // 将多个section的数据整合在一起
    std::vector<UGCSeasonEpisode> episodes;
    // 当前播放的视频在 episodes 中的索引值
    int currentIndex = -1;
    // 当前播放的视频去除了 episodes 中的标题项的索引值
    int currentIndexWithoutHeader = -1;
    // 前段合集的视频总数量
    int count;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UGCSeason, sections, id, mid, title, cover,
                                   intro, stat);

class VideoDetailResult {
public:
    std::string bvid;
    int aid;
    int videos;         // 视频数量
    int tid;            //分类ID
    int tname;          //分类名称
    int copyright;      //版权声明, 1: 有版权 2: 无版权
    std::string pic;    //封面图
    std::string title;  //标题
    std::string desc;   //简介
    int pubdate;        //发布时间
    int ctime;          //修改时间？
    int duration = 0;   //时长
    std::string redirect_url;
    VideoDetailRights rights;
    UserSimpleResult owner;
    VideoDetailPageListResult pages;
    VideoDetailStat stat;
    UGCSeason ugc_season;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoDetailResult& nlohmann_json_t) {
    if (nlohmann_json_j.contains("videos")) {
        nlohmann_json_j.at("videos").get_to(nlohmann_json_t.videos);
    }
    if (nlohmann_json_j.contains("pages")) {
        nlohmann_json_j.at("pages").get_to(nlohmann_json_t.pages);
    }
    if (nlohmann_json_j.contains("ugc_season")) {
        nlohmann_json_j.at("ugc_season").get_to(nlohmann_json_t.ugc_season);
    }
    if (nlohmann_json_j.contains("duration")) {
        nlohmann_json_j.at("duration").get_to(nlohmann_json_t.duration);
    }
    if (nlohmann_json_j.contains("redirect_url") &&
        nlohmann_json_j.at("redirect_url").is_string()) {
        nlohmann_json_j.at("redirect_url").get_to(nlohmann_json_t.redirect_url);
    } else {
        nlohmann_json_t.redirect_url = "";
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, bvid, aid,
                                             owner, title, pic, desc, pubdate,
                                             stat, rights, copyright));

    if (nlohmann_json_t.ugc_season.sections.empty()) return;
    auto& episodes = nlohmann_json_t.ugc_season.episodes;

    // 插入合集标题
    UGCSeasonEpisode seasonHeader;
    seasonHeader.title = nlohmann_json_t.ugc_season.title;
    seasonHeader.id    = 0;
    episodes.emplace_back(seasonHeader);

    // 将所有section的视频呢整合成一个列表
    for (auto& section : nlohmann_json_t.ugc_season.sections) {
        // 有多个section的情况下，插入section标题
        if (nlohmann_json_t.ugc_season.sections.size() > 1) {
            UGCSeasonEpisode sectionHeader;
            sectionHeader.title = section.title;
            sectionHeader.id    = 0;
            episodes.emplace_back(sectionHeader);
        }
        episodes.insert(episodes.end(), section.episodes.begin(),
                        section.episodes.end());
    }

    // 计算当前播放的视频在合集中的索引
    nlohmann_json_t.ugc_season.count = 0;
    for (size_t i = 0; i < episodes.size(); i++) {
        episodes[i].index = (int)i;
        if (episodes[i].aid == nlohmann_json_t.aid) {
            nlohmann_json_t.ugc_season.currentIndexWithoutHeader =
                nlohmann_json_t.ugc_season.count;
            nlohmann_json_t.ugc_season.currentIndex = (int)i;
        }
        if (episodes[i].id != 0) {
            nlohmann_json_t.ugc_season.count++;
        }
    }
}
inline void to_json(nlohmann::json& nlohmann_json_j,
                    const VideoDetailResult& nlohmann_json_t) {
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, bvid, aid, owner,
                                             title, pic, desc, pubdate, stat,
                                             copyright, videos, pages))
}

typedef std::vector<VideoDetailResult> VideoDetailListResult;

class UserDetailResult {
public:
    std::string mid, name, sex, rank, face, sign;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UserDetailResult, mid, name, sex, rank, face,
                                   sign);

class UserDetailResultWrapper {
public:
    unsigned int like_num, follower, article_count, archive_count;
    bool following;
    UserDetailResult card;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UserDetailResultWrapper, like_num, follower,
                                   article_count, archive_count, following,
                                   card);

class VideoDetailReplyPageResult {
public:
    int acount, count, num, size;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoDetailReplyPageResult, acount, count,
                                   num, size);

class VideoDetailReplyResult {
public:
    VideoDetailReplyPageResult page;
    VideoCommentListResult replies;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoDetailReplyResult& nlohmann_json_t) {
    if (!nlohmann_json_j.at("replies").is_null()) {
        nlohmann_json_j.at("replies").get_to(nlohmann_json_t.replies);
    }
    if (!nlohmann_json_j.at("page").is_null()) {
        nlohmann_json_j.at("page").get_to(nlohmann_json_t.page);
    }
}

class VideoDetailAllResult {
public:
    VideoDetailResult View;
    UserDetailResultWrapper Card;
    VideoDetailListResult Related;
    VideoDetailReplyResult Reply;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoDetailAllResult& nlohmann_json_t) {
    NLOHMANN_JSON_EXPAND(
        NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, View, Card, Related, Reply));
}

class VideoDUrl {
public:
    int order;
    int length;
    int size;
    std::string url;
    std::vector<std::string> backup_url;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoDUrl& nlohmann_json_t) {
    if (!nlohmann_json_j.at("backup_url").is_null()) {
        nlohmann_json_j.at("backup_url").get_to(nlohmann_json_t.backup_url);
    }
    NLOHMANN_JSON_EXPAND(
        NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, order, length, size, url));
}

class DashMedia {
public:
    int id;  // The format ID of Bilibili, corresponds to one resolution.
             // There may be items with the same ID but different bandwidth.
    std::string base_url;
    std::vector<std::string> backup_url;
    unsigned int bandwidth;
    int width, height;  // only for video
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      DashMedia& nlohmann_json_t) {
    if (nlohmann_json_j.contains("backup_url") &&
        !nlohmann_json_j.at("backup_url").is_null()) {
        nlohmann_json_j.at("backup_url").get_to(nlohmann_json_t.backup_url);
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, id, base_url,
                                             bandwidth, height, width));
}

class Dash {
public:
    unsigned int duration;
    float min_buffer_time;
    std::vector<DashMedia> video;
    std::vector<DashMedia> audio;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      Dash& nlohmann_json_t) {
    if (nlohmann_json_j.contains("audio") &&
        !nlohmann_json_j.at("audio").is_null()) {
        nlohmann_json_j.at("audio").get_to(nlohmann_json_t.audio);
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, duration,
                                             video, min_buffer_time));
}

class VideoUrlResult {
public:
    int quality;                                  //当前画质
    int timelength;                               // 视频长度
    std::vector<std::string> accept_description;  //可供选择的分辨率
    std::vector<int> accept_quality;  //可供选择的分辨率编号
    std::vector<VideoDUrl> durl;
    Dash dash;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoUrlResult& nlohmann_json_t) {
    if (nlohmann_json_j.contains("durl") &&
        !nlohmann_json_j.at("durl").is_null()) {
        nlohmann_json_j.at("durl").get_to(nlohmann_json_t.durl);
    }
    if (nlohmann_json_j.contains("dash") &&
        !nlohmann_json_j.at("dash").is_null()) {
        nlohmann_json_j.at("dash").get_to(nlohmann_json_t.dash);
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, quality,
                                             timelength, accept_description,
                                             accept_quality));
}

class SubtitleLine {
public:
    float from, to;
    int location;
    std::string content;
    float length = -1;  // 字幕在屏幕上渲染的宽度
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SubtitleLine, from, to, location, content);

typedef std::vector<SubtitleLine> SubtitleBody;

class SubtitleData {
public:
    std::string type, lang;
    bool genByAI;
    SubtitleBody body;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      SubtitleData& nlohmann_json_t) {
    if (nlohmann_json_j.contains("type") &&
        nlohmann_json_j.at("type").is_string()) {
        nlohmann_json_j.at("type").get_to(nlohmann_json_t.type);
    }
    nlohmann_json_t.genByAI = nlohmann_json_t.type == "AIsubtitle";
    if (nlohmann_json_j.contains("lang") &&
        nlohmann_json_j.at("lang").is_string()) {
        nlohmann_json_j.at("lang").get_to(nlohmann_json_t.lang);
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, body));
}

class VideoPageSubtitle {
public:
    std::string id_str;
    std::string lan;
    std::string lan_doc;
    std::string subtitle_url;
    SubtitleData data;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoPageSubtitle& nlohmann_json_t) {
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, id_str, lan,
                                             lan_doc, subtitle_url));
}

typedef std::vector<VideoPageSubtitle> VideoPageSubtitleList;

class VideoPageResult {
public:
    int online_count;
    int64_t last_play_time;
    int last_play_cid;
    VideoPageSubtitleList subtitles;
};
inline void from_json(const nlohmann::json& nlohmann_json_j,
                      VideoPageResult& nlohmann_json_t) {
    if (nlohmann_json_j.contains("subtitle")) {
        auto& sub = nlohmann_json_j.at("subtitle");
        if (sub.is_object() && sub.contains("subtitles")) {
            auto& subs = sub.at("subtitles");
            if (subs.is_array()) {
                subs.get_to(nlohmann_json_t.subtitles);
            }
        }
    }
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, online_count,
                                             last_play_time, last_play_cid));
}

class VideoRelation {
public:
    VideoRelation() {
        attention  = false;
        favorite   = false;
        season_fav = false;
        like       = false;
        dislike    = false;
        coin       = 0;
    }
    bool attention, favorite, season_fav, like, dislike;
    int coin;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoRelation, attention, favorite,
                                   season_fav, like, dislike, coin);

class VideoEpisodeRelationStat {
public:
    size_t coin, dm, like, reply, view;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoEpisodeRelationStat, coin, dm, like,
                                   reply, view);

class VideoEpisodeRelationUser {
public:
    size_t coin_number, favorite, is_original, like;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoEpisodeRelationUser, coin_number,
                                   favorite, is_original, like);

class VideoEpisodeRelation {
public:
    VideoEpisodeRelationStat stat;
    VideoEpisodeRelationUser user_community;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoEpisodeRelation, stat, user_community);

/// 三连返回的数据
class VideoTriple {
public:
    bool like, coin, fav;
    int multiply;  // 此次三连投了几个币
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoTriple, like, coin, fav, multiply);

class VideoOnlineTotal {
public:
    std::string total;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VideoOnlineTotal, total);

class Video {
public:
    int aid;
    std::string bvid;

    Video() = default;
    Video(const VideoDetailResult& r) : aid(r.aid), bvid(r.bvid) {}
    Video(const UserUploadedVideoResult& r) : aid(r.aid), bvid(r.bvid) {}
};

}  // namespace bilibili
